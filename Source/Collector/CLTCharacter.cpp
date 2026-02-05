// Fill out your copyright notice in the Description page of Project Settings.


#include "CLTCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"	
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Item/CLTItemBase.h"
#include "Item/CLTInventoryComponent.h"
#include "Item/CLTDoor.h"
#include "UI/CLTInventoryWidget.h"
#include "Blueprint/UserWidget.h" // For CreateWidget

// Sets default values
ACLTCharacter::ACLTCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeLocation(FVector(-20.0f, 0, -GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight()));
	GetMesh()->SetRelativeRotation(FRotator(0, -90.0f, 0));
	GetMesh()->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::WorldSpaceRepresentation;

	FirstMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstMesh"));
	FirstMesh->SetupAttachment(GetMesh());
	FirstMesh->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::FirstPerson;
	FirstMesh->SetOnlyOwnerSee(true);
	
	
	FirstCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstCamera"));
	FirstCamera->SetupAttachment(FirstMesh,FName("head"));
	FirstCamera->SetRelativeLocationAndRotation(FVector(2.8f,19.0f,0), FRotator(0, 90.0f, -90.0f));
	FirstCamera->bUsePawnControlRotation = true;
	FirstCamera->FirstPersonFieldOfView = 70.0f;
	FirstCamera->FirstPersonScale = 0.6f;
	FirstCamera->bEnableFirstPersonFieldOfView = true;
	FirstCamera->bEnableFirstPersonScale = true;

	StimuliSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("StimuliSource"));

	GetCharacterMovement()->MaxWalkSpeed = 350.0f;

	SetGenericTeamId(1);
}

// Called when the game starts or when spawned
void ACLTCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocallyControlled() && InventoryWidgetClass)
	{
		InventoryWidget = CreateWidget<UCLTInventoryWidget>(GetWorld(), InventoryWidgetClass);
		if (InventoryWidget)
		{
			UCLTInventoryComponent* InventoryComp = FindComponentByClass<UCLTInventoryComponent>();
			if (InventoryComp)
			{
				InventoryWidget->Init(InventoryComp);
			}
			InventoryWidget->AddToViewport();
			InventoryWidget->SetVisibility(ESlateVisibility::Visible); // Always Visible
		}
	}

	if (IsLocallyControlled() && MainHUDWidgetClass)
	{
		MainHUDWidget = CreateWidget<UUserWidget>(GetWorld(), MainHUDWidgetClass);
		if (MainHUDWidget)
		{
			MainHUDWidget->AddToViewport();
		}
	}
}

// Called every frame
void ACLTCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bSprint)
	{
		CurrentStamina = CurrentStamina - (UseStamina * UGameplayStatics::GetWorldDeltaSeconds(GetWorld()));
		if (CurrentStamina <= 0.0f)
		{
			CurrentStamina = 0.0f;
			StopSprint();
			GetWorldTimerManager().ClearTimer(StaminaChargingTimer);
		}
	}
	else if(bCanCharging)
	{
		if (CurrentStamina < MaxStamina)
		{
			CurrentStamina = CurrentStamina + (UseStamina * UGameplayStatics::GetWorldDeltaSeconds(GetWorld()));
			if (CurrentStamina > MaxStamina)
			{
				CurrentStamina = MaxStamina;
			}
		}
	}
}

// Called to bind functionality to input
void ACLTCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* UIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (UIC)
	{
		UIC->BindAction(IA_Sprint, ETriggerEvent::Triggered, this, &ACLTCharacter::StartSprint);
		UIC->BindAction(IA_Sprint, ETriggerEvent::Completed, this, &ACLTCharacter::StopSprint);
		UIC->BindAction(IA_Walk, ETriggerEvent::Triggered, this, &ACLTCharacter::StartWalk);
		UIC->BindAction(IA_Walk, ETriggerEvent::Completed, this, &ACLTCharacter::StopWalk);
		UIC->BindAction(IA_GetItem, ETriggerEvent::Started, this, &ACLTCharacter::GetItem);
		UIC->BindAction(IA_OpenDoor, ETriggerEvent::Started, this, &ACLTCharacter::OpenDoor);
		UIC->BindAction(IA_UseItemSlot, ETriggerEvent::Started, this, &ACLTCharacter::OnUseItemSlot);
		UIC->BindAction(IA_UseItemSlot, ETriggerEvent::Started, this, &ACLTCharacter::OnUseItemSlot);
		UIC->BindAction(IA_Drop, ETriggerEvent::Started, this, &ACLTCharacter::DropItem);
		
		// Assuming we re-use IA_Drop or a new IA_Inventory for toggling. 
		// For now, I'll allow binding via a new input or just hook into an existing one if available. 
		// NOTE: User didn't specify an input action for Inventory. I will add a bind if I find a suitable place or just leave the function available for blueprint binding.
		// Detailed plan said: "Top-Level Input: Add logic... e.g valid IA_Inventory".
		// I will rely on the user to Bind `ToggleInventory` in Blueprint or add a new Input Action later.
	}
}

void ACLTCharacter::ToggleInventory()
{
	if (InventoryWidget)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		if (PC)
		{
			if (PC->bShowMouseCursor)
			{
				// Switch to Game Mode (Hide Cursor, but Widget stays visible)
				PC->SetShowMouseCursor(false);
				PC->SetInputMode(FInputModeGameOnly());
			}
			else
			{
				// Switch to UI Mode (Show Cursor to interact with Widget)
				PC->SetShowMouseCursor(true);
				PC->SetInputMode(FInputModeGameAndUI());
			}
		}
	}
}

void ACLTCharacter::Move(float Forward, float Right)
{
	const FRotator CharacterRotation = FRotator(GetController()->GetControlRotation());
	const FRotator CharacterRotationYaw = FRotator(0, CharacterRotation.Yaw, 0);

	const FVector ForwardVector = UKismetMathLibrary::GetForwardVector(CharacterRotationYaw);
	AddMovementInput(ForwardVector, Forward);

	const FVector RightVector = UKismetMathLibrary::GetRightVector(CharacterRotationYaw);
	AddMovementInput(RightVector, Right);
}

void ACLTCharacter::Aim(float Pitch, float Yaw)
{
	AddControllerPitchInput(Pitch);
	AddControllerYawInput(Yaw);
}

void ACLTCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACLTCharacter, bSprint);
	DOREPLIFETIME(ACLTCharacter, bCanCharging);
	DOREPLIFETIME(ACLTCharacter, CurrentStamina);
	DOREPLIFETIME(ACLTCharacter, MaxStamina);
	DOREPLIFETIME(ACLTCharacter, UseStamina);
	DOREPLIFETIME(ACLTCharacter, CurrentHP);
	DOREPLIFETIME(ACLTCharacter, MaxHP);
	DOREPLIFETIME(ACLTCharacter, bLookWhisper);
	DOREPLIFETIME(ACLTCharacter, bWalk);
}

void ACLTCharacter::SetGenericTeamId(const FGenericTeamId& InTeamID)
{
	TeamID = InTeamID;
}

FGenericTeamId ACLTCharacter::GetGenericTeamId() const
{
	return TeamID;
}

void ACLTCharacter::SpawnFootSound()
{
	if (bWalk)
	{
		return;
	}

	UGameplayStatics::SpawnSoundAtLocation(
		GetWorld(),
		FootSound,
		GetActorLocation()
	);

	UAISense_Hearing::ReportNoiseEvent(
		GetWorld(),
		GetActorLocation(),
		1.0f,
		this,
		-1
	);
}

void ACLTCharacter::GetItem()
{	
	if (GetOwner()->HasAuthority())
	{
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
		FVector Start = GetMesh()->GetSocketLocation(FName("root"));
		FVector End = Start - 20.0f;
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
		TArray<AActor*> IngnoreActors;
		FHitResult HitResult;

		bool bResult = UKismetSystemLibrary::SphereTraceSingleForObjects(
			GetWorld(),
			Start,
			End,
			40.0f,
			ObjectTypes,
			false,
			IngnoreActors,
			EDrawDebugTrace::ForDuration,
			HitResult,
			true,
			FLinearColor::Red,
			FLinearColor::Green,
			3.0f
		);
		ACLTItemBase* ScanItem = Cast<ACLTItemBase>(HitResult.GetActor());
		UCLTInventoryComponent* InventoryComponent = FindComponentByClass<UCLTInventoryComponent>();

		if (ScanItem && InventoryComponent)
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit %s"), *ScanItem->ItemName.ToString());

			if (InventoryComponent->AddItem(ScanItem->ItemName))
			{
				ScanItem->Destroy();
				for (int i = 0; i < InventoryComponent->Inventory.Num(); i++)
				{
					UE_LOG(LogTemp, Warning, TEXT("현재 인벤토리 아이템: %s"), *InventoryComponent->Inventory[i].Name.ToString());
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Inventory is Full"));
			}
		}
	}
	else
	{
		C2S_GetItem();
	}
}

void ACLTCharacter::C2S_GetItem_Implementation()
{
	GetItem();
}

void ACLTCharacter::StartSprint()
{
	bSprint = true;
	bCanCharging = false;
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	C2S_StartSprint();
}

void ACLTCharacter::C2S_StartSprint_Implementation()
{
	bSprint = true;
	bCanCharging = false;
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
}

void ACLTCharacter::StopSprint()
{
	bSprint = false;
	GetCharacterMovement()->MaxWalkSpeed = 350.0f;

	GetWorldTimerManager().SetTimer(
		StaminaChargingTimer,
		this,
		&ACLTCharacter::CanChargingStamina,
		3.0f,
		false
	);
	C2S_StopSprint();
}

void ACLTCharacter::C2S_StopSprint_Implementation()
{
	bSprint = false;
	GetCharacterMovement()->MaxWalkSpeed = 350.0f;

	GetWorldTimerManager().SetTimer(
		StaminaChargingTimer,
		this,
		&ACLTCharacter::CanChargingStamina,
		3.0f,
		false
	);
}

void ACLTCharacter::StartWalk()
{
	bWalk = true;
	GetCharacterMovement()->MaxWalkSpeed = 150.0f;
	C2S_StartWalk();
}

void ACLTCharacter::C2S_StartWalk_Implementation()
{
	bWalk = true;
	GetCharacterMovement()->MaxWalkSpeed = 150.0f;
}

void ACLTCharacter::StopWalk()
{
	bWalk = false;
	GetCharacterMovement()->MaxWalkSpeed = 350.0f;
	C2S_StopWalk();
}

void ACLTCharacter::C2S_StopWalk_Implementation()
{
	bWalk = false;
	GetCharacterMovement()->MaxWalkSpeed = 350.0f;
}

void ACLTCharacter::CanChargingStamina()
{
	bCanCharging = true;
	C2S_CanChargingStamina();
}

void ACLTCharacter::C2S_CanChargingStamina_Implementation()
{
	bCanCharging = true;
}

void ACLTCharacter::OpenDoor()
{
	if (GetOwner()->HasAuthority())
	{
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
		FVector Start = FirstCamera->GetComponentLocation();
		FVector End = Start + (FirstCamera->GetForwardVector() * 100.0f);
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
		TArray<AActor*> IngnoreActors;
		FHitResult HitResult;

		bool bResult = UKismetSystemLibrary::LineTraceSingleForObjects(
			GetWorld(),
			Start,
			End,
			ObjectTypes,
			false,
			IngnoreActors,
			EDrawDebugTrace::ForDuration, // Debug draw enabled
			HitResult,
			true,
			FLinearColor::Blue, // Distinction color
			FLinearColor::Green,
			3.0f
		);

		ACLTDoor* ScanDoor = Cast<ACLTDoor>(HitResult.GetActor());
		if (ScanDoor)
		{
			ScanDoor->Open(this);
		}
	}
	else
	{
		C2S_OpenDoor();
	}
}

void ACLTCharacter::C2S_OpenDoor_Implementation()
{
	OpenDoor();
}

void ACLTCharacter::OnUseItemSlot(const FInputActionValue& Value)
{
    // Axis Value (1.0, 2.0, 3.0, 4.0) returned from input
    float InputValue = Value.Get<float>();
    int32 SlotIndex = FMath::RoundToInt(InputValue) - 1; // Convert 1-based to 0-based

    CurrentSelectedSlotIndex = SlotIndex;
    UseItemInSlot(SlotIndex);
}

void ACLTCharacter::UseItemInSlot(int32 SlotIndex)
{
    if (GetOwner()->HasAuthority())
    {
        UCLTInventoryComponent* InventoryComponent = FindComponentByClass<UCLTInventoryComponent>();
        if (InventoryComponent)
        {
            // Check valid index
            if (InventoryComponent->Inventory.IsValidIndex(SlotIndex))
            {
                const FItemData& ItemData = InventoryComponent->Inventory[SlotIndex];
                

                // Check if item is not empty
                if (!ItemData.Name.IsEqual(FName("Empty")))
                {
                    UE_LOG(LogTemp, Log, TEXT("Using Item in Slot %d: %s"), SlotIndex + 1, *ItemData.Name.ToString());
                    
                    // Here you can add logic to actually use the item
                    // E.g., EquipItem(ItemData) or ConsumeItem(ItemData)
                }
                else
                {
                   UE_LOG(LogTemp, Warning, TEXT("Slot %d is Empty."), SlotIndex + 1);
                }
				
				CurrentSelectedSlotIndex = SlotIndex;
            }
        }
    }
    else
    {
        C2S_UseItemInSlot(SlotIndex);
    }
}

void ACLTCharacter::C2S_UseItemInSlot_Implementation(int32 SlotIndex)
{
    UseItemInSlot(SlotIndex);
}

void ACLTCharacter::DropItem()
{
	if (CurrentSelectedSlotIndex < 0)
	{
		return;
	}
	
	C2S_DropItem(CurrentSelectedSlotIndex);
}

void ACLTCharacter::C2S_DropItem_Implementation(int32 SlotIndex)
{
	UCLTInventoryComponent* InventoryComponent = FindComponentByClass<UCLTInventoryComponent>();
	if (InventoryComponent)
	{
		FName RemovedItemName = InventoryComponent->RemoveItem(SlotIndex);
		if (!RemovedItemName.IsNone())
		{
            FVector TraceStart = GetActorLocation() + (GetActorForwardVector() * 100.0f);
            FVector TraceEnd = TraceStart - FVector(0.0f, 0.0f, 500.0f);
            FHitResult HitResult;
            FVector SpawnLocation = TraceStart;

            bool bHit = GetWorld()->LineTraceSingleByChannel(
                HitResult,
                TraceStart,
                TraceEnd,
                ECC_Visibility
            );

            if (bHit)
            {
                SpawnLocation = HitResult.Location;
            }

			FTransform SpawnTransform(GetActorRotation(), SpawnLocation);
			
			ACLTItemBase* SpawnedItem = GetWorld()->SpawnActorDeferred<ACLTItemBase>(ACLTItemBase::StaticClass(), SpawnTransform, this, this);
			if (SpawnedItem)
			{
				SpawnedItem->ItemName = RemovedItemName;
				// Optional: Load Mesh from Data Table if needed (assuming ItemName is RowName)
				if (InventoryComponent->ItemDataTable)
				{
					FItemData* ItemData = InventoryComponent->ItemDataTable->FindRow<FItemData>(RemovedItemName, TEXT("Drop Item"));
					if (ItemData && ItemData->Mesh && SpawnedItem->Item)
					{
						SpawnedItem->Item->SetStaticMesh(ItemData->Mesh);
						SpawnedItem->ItemData = *ItemData; // Copy data
					}
				}
				
				UGameplayStatics::FinishSpawningActor(SpawnedItem, SpawnTransform);
			}
		}
	}
}

