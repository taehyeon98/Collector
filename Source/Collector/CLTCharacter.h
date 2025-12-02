// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GenericTeamAgentInterface.h"
#include "CLTCharacter.generated.h"

class UInputAction;
class UAIPerceptionStimuliSourceComponent;

UCLASS()
class COLLECTOR_API ACLTCharacter : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACLTCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category = Character)
	TObjectPtr<class USkeletalMeshComponent> FirstMesh;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category = Character)
	TObjectPtr<class UCameraComponent> FirstCamera;

	UFUNCTION(BlueprintCallable)
	void Move(float Forward, float Right);

	UFUNCTION(BlueprintCallable)
	void Aim(float Pitch, float Yaw);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Data)
	uint8 bSprint : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	FGenericTeamId TeamID;

	UPROPERTY(Category = Character, EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAIPerceptionStimuliSourceComponent> StimuliSource;

	/** Assigns Team Agent to given TeamID */
	virtual void SetGenericTeamId(const FGenericTeamId& InTeamID) override;

	/** Retrieve team identifier in form of FGenericTeamId */
	virtual FGenericTeamId GetGenericTeamId() const override;
};
