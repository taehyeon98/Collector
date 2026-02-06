#include "CLTNetworkSubsystem.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "Interfaces/IPv4/IPv4Address.h"
#include "Common/TcpSocketBuilder.h"
#include "Kismet/GameplayStatics.h"

void UCLTNetworkSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	ListenSocket = nullptr;
	ClientSocket = nullptr;
	ReceiveBuffer.SetNumUninitialized(1024); // Initial buffer size
	UE_LOG(LogTemp, Log, TEXT("CLTNetworkSubsystem Initialized"));
}

void UCLTNetworkSubsystem::Deinitialize()
{
	ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
	if (SocketSubsystem)
	{
		if (ListenSocket)
		{
			ListenSocket->Close();
			SocketSubsystem->DestroySocket(ListenSocket);
			ListenSocket = nullptr;
		}

		if (ClientSocket)
		{
			ClientSocket->Close();
			SocketSubsystem->DestroySocket(ClientSocket);
			ClientSocket = nullptr;
		}

		for (FSocket* Socket : ConnectedClients)
		{
			if (Socket)
			{
				Socket->Close();
				SocketSubsystem->DestroySocket(Socket);
			}
		}
	}
	ConnectedClients.Empty();

	Super::Deinitialize();
}

void UCLTNetworkSubsystem::Tick(float DeltaTime)
{
	// Server Logic: Accept new connections
	if (ListenSocket)
	{
		AcceptNewConnections();
		
		// Handle data from connected clients
		// Iterating inversely to remove if needed (though not implementing robust disconnect logic here)
		for (FSocket* Client : ConnectedClients)
		{
			ReceiveData(Client, true);
		}
	}

	// Client Logic: Receive data from server
	if (ClientSocket && ClientSocket->GetConnectionState() == SCS_Connected)
	{
		ReceiveData(ClientSocket, false);
	}
}

TStatId UCLTNetworkSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UCLTNetworkSubsystem, STATGROUP_Tickables);
}

bool UCLTNetworkSubsystem::StartTcpServer(int32 Port)
{
	if (ListenSocket) return false;

	ListenSocket = FTcpSocketBuilder(TEXT("CLTLoginServer"))
		.AsReusable()
		.BoundToPort(Port)
		.Listening(8)
		.Build();

	if (ListenSocket)
	{
		UE_LOG(LogTemp, Log, TEXT("TCP Server Started on Port %d"), Port);
		return true;
	}
	
	UE_LOG(LogTemp, Error, TEXT("Failed to start TCP Server on Port %d"), Port);
	return false;
}

bool UCLTNetworkSubsystem::ConnectToTcpServer(FString IPAddress, int32 Port)
{
	if (ClientSocket)
	{
		ClientSocket->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(ClientSocket);
		ClientSocket = nullptr;
	}

	FIPv4Address IP;
	if (!FIPv4Address::Parse(IPAddress, IP))
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid IP Address: %s"), *IPAddress);
		return false;
	}

	TSharedRef<FInternetAddr> Addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	Addr->SetIp(IP.Value);
	Addr->SetPort(Port);

	ClientSocket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("CLTLoginClient"), false);
	
	if (ClientSocket->Connect(*Addr))
	{
		UE_LOG(LogTemp, Log, TEXT("Connected to TCP Server %s:%d"), *IPAddress, Port);
		return true;
	}

	UE_LOG(LogTemp, Error, TEXT("Failed to connect to %s:%d"), *IPAddress, Port);
	return false;
}

void UCLTNetworkSubsystem::SendLoginPacket(FString UserID)
{
	if (!ClientSocket || ClientSocket->GetConnectionState() != SCS_Connected) return;

	FPacketLoginRequest ReqPacket;
	FString SafeID = UserID.Left(31); // Ensure it fits
	
	// String to char array conversion
	FTCHARToUTF8 Converter(*SafeID);
	FMemory::Memcpy(ReqPacket.UserID, Converter.Get(), Converter.Length());
	ReqPacket.UserID[Converter.Length()] = '\0'; // Null terminate

	int32 BytesSent = 0;
	ClientSocket->Send((uint8*)&ReqPacket, sizeof(FPacketLoginRequest), BytesSent);
	UE_LOG(LogTemp, Log, TEXT("Sent Login Request for ID: %s"), *SafeID);
}

void UCLTNetworkSubsystem::AcceptNewConnections()
{
	if (!ListenSocket) return;

	bool bPending = false;
	ListenSocket->HasPendingConnection(bPending);

	if (bPending)
	{
		TSharedRef<FInternetAddr> RemoteAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
		FSocket* NewClient = ListenSocket->Accept(*RemoteAddr, TEXT("CLTClientConnection"));
		
		if (NewClient)
		{
			ConnectedClients.Add(NewClient);
			UE_LOG(LogTemp, Log, TEXT("Accepted Connection from %s"), *RemoteAddr->ToString(true));
		}
	}
}

void UCLTNetworkSubsystem::ReceiveData(FSocket* Socket, bool bIsServer)
{
	if (!Socket) return;

	uint32 PendingDataSize = 0;
	if (Socket->HasPendingData(PendingDataSize) && PendingDataSize > 0)
	{
		if (ReceiveBuffer.Num() < (int32)PendingDataSize)
		{
			ReceiveBuffer.SetNumUninitialized(PendingDataSize);
		}

		int32 BytesRead = 0;
		if (Socket->Recv(ReceiveBuffer.GetData(), PendingDataSize, BytesRead))
		{
			// Create a temporary array to pass to ProcessPacket
			TArray<uint8> PacketData;
			PacketData.Append(ReceiveBuffer.GetData(), BytesRead);
			ProcessPacket(Socket, PacketData, bIsServer);
		}
	}
}

void UCLTNetworkSubsystem::ProcessPacket(FSocket* SenderSocket, const TArray<uint8>& PacketData, bool bIsServer)
{
	if (PacketData.Num() < sizeof(FPacketHeader)) return;

	const FPacketHeader* Header = reinterpret_cast<const FPacketHeader*>(PacketData.GetData());

	if (bIsServer)
	{
		// Server Handling
		if (Header->PacketType == EPacketType::Packet_LoginReq)
		{
			if (PacketData.Num() >= sizeof(FPacketLoginRequest))
			{
				const FPacketLoginRequest* Req = reinterpret_cast<const FPacketLoginRequest*>(PacketData.GetData());
				FString ReceivedID = UTF8_TO_TCHAR(Req->UserID);
				UE_LOG(LogTemp, Log, TEXT("Server: Received Login Request from %s"), *ReceivedID);

				// Always success for demo
				FPacketLoginResponse ResPacket;
				ResPacket.bSuccess = true;

				int32 BytesSent = 0;
				SenderSocket->Send((uint8*)&ResPacket, sizeof(FPacketLoginResponse), BytesSent);
				UE_LOG(LogTemp, Log, TEXT("Server: Sent Login Response (Success)"));
			}
		}
	}
	else
	{
		// Client Handling
		if (Header->PacketType == EPacketType::Packet_LoginRes)
		{
			if (PacketData.Num() >= sizeof(FPacketLoginResponse))
			{
				const FPacketLoginResponse* Res = reinterpret_cast<const FPacketLoginResponse*>(PacketData.GetData());
				UE_LOG(LogTemp, Log, TEXT("Client: Received Login Response Success=%d"), Res->bSuccess);
				
				OnLoginResult.Broadcast(Res->bSuccess);
			}
		}
	}
}
