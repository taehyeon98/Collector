#include "CLTLoginWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Kismet/GameplayStatics.h"
#include "../Network/CLTNetworkSubsystem.h"

void UCLTLoginWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (Btn_Host)
	{
		Btn_Host->OnClicked.AddDynamic(this, &UCLTLoginWidget::OnHostClicked);
	}

	if (Btn_Login)
	{
		Btn_Login->OnClicked.AddDynamic(this, &UCLTLoginWidget::OnLoginClicked);
	}

	UGameInstance* GI = GetGameInstance();
	if (GI)
	{
		UCLTNetworkSubsystem* NetworkSubsystem = GI->GetSubsystem<UCLTNetworkSubsystem>();
		if (NetworkSubsystem)
		{
			NetworkSubsystem->OnLoginResult.AddDynamic(this, &UCLTLoginWidget::OnLoginResult);
		}
	}
}

void UCLTLoginWidget::OnHostClicked()
{
	UGameInstance* GI = GetGameInstance();
	if (!GI) return;

	UCLTNetworkSubsystem* NetworkSubsystem = GI->GetSubsystem<UCLTNetworkSubsystem>();
	if (NetworkSubsystem)
	{
		// Start TCP Server on port 30000
		NetworkSubsystem->StartTcpServer(30000);
	}

	// Open Map as Listen Server (UDP 7777 will be opened by default UE networking)
	// Assuming "L_Main" or whatever map the user has. I will open "L_Lobby" or similar if they have it, 
	// or current level with ?listen.
	// For now, I'll use a placeholder name "L_Level1" based on common naming, or ask user?
	// User didn't specify map name. I will set it to open "ThirdPersonMap" or "Level1" by default, 
	// or just "L_Level1". 
	// SAFEST: Open "First" (User specified).
	UGameplayStatics::OpenLevel(this, FName("First"), true, TEXT("listen"));
}

void UCLTLoginWidget::OnLoginClicked()
{
	UGameInstance* GI = GetGameInstance();
	if (!GI) return;

	UCLTNetworkSubsystem* NetworkSubsystem = GI->GetSubsystem<UCLTNetworkSubsystem>();
	if (NetworkSubsystem)
	{
		FString IP = Input_IP ? Input_IP->GetText().ToString() : TEXT("127.0.0.1");
		IP = IP.TrimStartAndEnd(); // Remove whitespace
		if (IP.IsEmpty()) IP = TEXT("127.0.0.1");

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Attempting to connect to %s..."), *IP));

		// 1. Connect TCP
		if (NetworkSubsystem->ConnectToTcpServer(IP, 30000))
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("TCP Connected. Sending Login Packet..."));
			// 2. Send Login Packet (Hardcoded ID for now)
			NetworkSubsystem->SendLoginPacket(TEXT("Player_") + FString::FromInt(FMath::RandRange(0, 1000)));
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Failed to Connect TCP. Check Server or IP."));
		}
	}
}

void UCLTLoginWidget::OnLoginResult(bool bSuccess)
{
	if (bSuccess)
	{
		UE_LOG(LogTemp, Log, TEXT("Login Success! Connecting to UDP Game server..."));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Login Success! Joining Game..."));
		
		FString IP = Input_IP ? Input_IP->GetText().ToString() : TEXT("127.0.0.1");
		IP = IP.TrimStartAndEnd();
		if (IP.IsEmpty()) IP = TEXT("127.0.0.1");

		// 3. Connect UDP (ClientTravel)
		// Standard UE Connection to Port 7777 (Default)
		UGameplayStatics::OpenLevel(this, FName(*IP));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Login Failed."));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Login Denied by Server."));
	}
}
