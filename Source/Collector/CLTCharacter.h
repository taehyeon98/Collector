// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GenericTeamAgentInterface.h"
#include "Item/CLTItemBase.h"
#include "InputActionValue.h"
#include "CLTCharacter.generated.h"

class UInputAction;
class UAIPerceptionStimuliSourceComponent;
class ACLTItemBase;

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
	void GetItem();

	UFUNCTION(Server, Reliable)
	void C2S_GetItem();
	void C2S_GetItem_Implementation();

	UFUNCTION(BlueprintCallable)
	void StartSprint();

	UFUNCTION(Server, Reliable)
	void C2S_StartSprint();
	void C2S_StartSprint_Implementation();

	UFUNCTION(BlueprintCallable)
	void StopSprint();

	UFUNCTION(Server,Reliable)
	void C2S_StopSprint();
	void C2S_StopSprint_Implementation();

	UFUNCTION(BlueprintCallable)
	void StartWalk();

	UFUNCTION(Server, Reliable)
	void C2S_StartWalk();
	void C2S_StartWalk_Implementation();

	UFUNCTION(BlueprintCallable)
	void StopWalk();

	UFUNCTION(Server, Reliable)
	void C2S_StopWalk();
	void C2S_StopWalk_Implementation();

	UFUNCTION(BlueprintCallable)
	void CanChargingStamina();

	UFUNCTION(Server,Reliable)
	void C2S_CanChargingStamina();
	void C2S_CanChargingStamina_Implementation();

	UFUNCTION(BlueprintCallable)
	void OpenDoor();

	UFUNCTION(Server, Reliable)
	void C2S_OpenDoor();
	void C2S_OpenDoor_Implementation();

    // Inventory Usage
    void OnUseItemSlot(const FInputActionValue& Value);

    UFUNCTION(BlueprintCallable)
    void UseItemInSlot(int32 SlotIndex);

    UFUNCTION(Server, Reliable)
    void C2S_UseItemInSlot(int32 SlotIndex);
    void C2S_UseItemInSlot_Implementation(int32 SlotIndex);

	UFUNCTION(BlueprintCallable)
	void DropItem();

	UFUNCTION(Server, Reliable)
	void C2S_DropItem(int32 SlotIndex);
	void C2S_DropItem_Implementation(int32 SlotIndex);

	int32 CurrentSelectedSlotIndex = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	TObjectPtr<class USoundBase> FootSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	FGenericTeamId TeamID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	TObjectPtr<UInputAction> IA_Sprint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	TObjectPtr<UInputAction> IA_Walk;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	TObjectPtr<UInputAction> IA_GetItem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	TObjectPtr<UInputAction> IA_OpenDoor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	TObjectPtr<UInputAction> IA_UseItemSlot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	TObjectPtr<UInputAction> IA_Drop;

	UPROPERTY(Category = Character, EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAIPerceptionStimuliSourceComponent> StimuliSource;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data, Replicated)
	uint8 bSprint : 1 = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data, Replicated)
	uint8 bWalk : 1 = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data, Replicated)
	uint8 bCanCharging : 1 = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data, Replicated)
	float CurrentStamina = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data, Replicated)
	float MaxStamina = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data, Replicated)
	float UseStamina = 15.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data, Replicated)
	float CurrentHP = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data, Replicated)
	float MaxHP = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data, Replicated)
	int32 PlayerGold = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data, Replicated)
	uint8 bLookWhisper : 1 = false;

	FTimerHandle StaminaChargingTimer;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** Assigns Team Agent to given TeamID */
	virtual void SetGenericTeamId(const FGenericTeamId& InTeamID) override;

	/** Retrieve team identifier in form of FGenericTeamId */
	virtual FGenericTeamId GetGenericTeamId() const override;
};
