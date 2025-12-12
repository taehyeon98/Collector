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

	UFUNCTION(BlueprintCallable)
	void SpawnFootSound();

	UFUNCTION(BlueprintCallable)
	void StartSprint();

	UFUNCTION(BlueprintCallable)
	void StopSprint();

	UFUNCTION(BlueprintCallable)
	void CanChargingStamina();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	TObjectPtr<class USoundBase> FootSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	FGenericTeamId TeamID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	TObjectPtr<UInputAction> IA_Sprint;

	UPROPERTY(Category = Character, EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAIPerceptionStimuliSourceComponent> StimuliSource;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	uint8 bSprint : 1 = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	uint8 bCanCharging : 1 = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	float CurrentStamina = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	float MaxStamina = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	float UseStamina = 15.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	float CurrentHP = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	float MaxHP = 100;

	FTimerHandle StaminaChargingTimer;

	/** Assigns Team Agent to given TeamID */
	virtual void SetGenericTeamId(const FGenericTeamId& InTeamID) override;

	/** Retrieve team identifier in form of FGenericTeamId */
	virtual FGenericTeamId GetGenericTeamId() const override;
};
