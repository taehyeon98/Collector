// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CLTDoor.generated.h"

UCLASS()
class COLLECTOR_API ACLTDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACLTDoor();

public:	
	/** Called by the character to execute the door logic (e.g. Teleport). */
	void Open(APawn* InstigatorPawn);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UStaticMeshComponent> DoorMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UBoxComponent> TriggerZone;

	/** The location to teleport the character to when entering. Relative to Actor. */
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Door Properties", meta = (MakeEditWidget = true))
	FVector TargetLocation;

	/** Optional: Reference to another Door or Actor to teleport to (overrides TargetLocation if set). */
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Door Properties")
	TObjectPtr<AActor> TargetActor;

	/** Sound to play on interact */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door Properties")
	TObjectPtr<USoundBase> DoorSound;

	/** Distance to teleport forward from character's current position */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door Properties")
	float TeleportDistance = 300.0f;
};
