// Fill out your copyright notice in the Description page of Project Settings.


#include "CLTDoor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"


// Sets default values
ACLTDoor::ACLTDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	DoorMesh->SetupAttachment(RootComponent);

	TriggerZone = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerZone"));
	TriggerZone->SetupAttachment(RootComponent);
	TriggerZone->SetCollisionProfileName(TEXT("Trigger"));
}

void ACLTDoor::Open(APawn* InstigatorPawn)
{
	if (!InstigatorPawn) return;

	if (DoorSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DoorSound, GetActorLocation());
	}

	// Calculate destination based on Character's Forward direction
	// This allows passing through the door regardless of which side the player is on.
	FVector ForwardVec = InstigatorPawn->GetActorForwardVector();
	
	// Ensure we are only moving horizontally if desired, but ActorForwardVector is usually fine for Characters.
	// If the character is looking down, we might want to flatten the vector, but typically we want to follow the body facing.
	
	FVector TeleportDest = InstigatorPawn->GetActorLocation() + (ForwardVec * TeleportDistance);

	InstigatorPawn->TeleportTo(TeleportDest, InstigatorPawn->GetActorRotation());
}
