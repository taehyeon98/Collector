#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PacketDefinitions.h"
#include "CLTNetworkSubsystem.generated.h"

class FSocket;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLoginResultDelegate, bool, bSuccess);

/**
 * Subsystem to handle TCP connections for Login.
 */
UCLASS()
class COLLECTOR_API UCLTNetworkSubsystem : public UGameInstanceSubsystem, public FTickableGameObject
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	// FTickableGameObject Interface
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;
	virtual bool IsTickable() const override { return true; }

	// Functions to be called from UI
	UFUNCTION(BlueprintCallable, Category = "Network")
	bool StartTcpServer(int32 Port);

	UFUNCTION(BlueprintCallable, Category = "Network")
	bool ConnectToTcpServer(FString IPAddress, int32 Port);

	UFUNCTION(BlueprintCallable, Category = "Network")
	void SendLoginPacket(FString UserID);

	UPROPERTY(BlueprintAssignable, Category = "Network")
	FOnLoginResultDelegate OnLoginResult;

private:
	// Server side
	FSocket* ListenSocket;
	TArray<FSocket*> ConnectedClients;

	// Client side
	FSocket* ClientSocket;

	// Buffer for receiving data
	TArray<uint8> ReceiveBuffer;

	void AcceptNewConnections();
	void ReceiveData(FSocket* Socket, bool bIsServer);
	
	void ProcessPacket(FSocket* SenderSocket, const TArray<uint8>& PacketData, bool bIsServer);
};
