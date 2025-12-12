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
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Hearing.h"

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

void ACLTCharacter::StartSprint()
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
}

void ACLTCharacter::CanChargingStamina()
{
	bCanCharging = true;
}

