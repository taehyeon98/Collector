// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CLTMonsterBase.generated.h"

UENUM(BlueprintType)
enum class EMonsterState : uint8
{
	Normal = 0 UMETA(DisplayName = "Normal"),
	Chase = 1 UMETA(DisplayName = "Chase"),
	Battle = 2 UMETA(DisplayName = "Battle"),
	Death = 3 UMETA(DisplayName = "Death")
};

UCLASS()
class COLLECTOR_API ACLTMonsterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACLTMonsterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Character)
	float CurrentHP = 100;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Character)
	float MaxHP = 100;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Character)
	EMonsterState CurrentState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	TObjectPtr<UParticleSystem> BloodEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	TObjectPtr<UAnimMontage> DeathMontage;
public:
	UFUNCTION(BlueprintCallable)
	void Dead();

	UFUNCTION(BlueprintCallable)
	void DeadEnd();

	UFUNCTION(BlueprintCallable)
	virtual void SpawnHitEffect(FHitResult Hit);

	UFUNCTION(BlueprintCallable)
	void ChangeSpeed(float NewMaxSpeed);

	void SetState(EMonsterState NewState);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	FORCEINLINE const EMonsterState GetCurrentState() { return CurrentState; }

};
