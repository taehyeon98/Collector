// Fill out your copyright notice in the Description page of Project Settings.


#include "CLTMonsterBase.h"
#include "Engine/DamageEvents.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CLTMonsterController.h"

// Sets default values
ACLTMonsterBase::ACLTMonsterBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	GetCharacterMovement()->MaxWalkSpeed = 80.0f;
}

// Called when the game starts or when spawned
void ACLTMonsterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACLTMonsterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACLTMonsterBase::Dead()
{
	FName SectionName = FName(FString::Printf(TEXT("Default")));
	PlayAnimMontage(DeathMontage, 1.0f, SectionName);
}

void ACLTMonsterBase::DeadEnd()
{
	GetController()->SetActorEnableCollision(false);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetSimulatePhysics(true);
}

void ACLTMonsterBase::SpawnHitEffect(FHitResult Hit)
{
	if (BloodEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			BloodEffect,
			Hit.ImpactPoint,
			Hit.ImpactNormal.Rotation()
		);
	}
}

void ACLTMonsterBase::ChangeSpeed(float NewMaxSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = NewMaxSpeed;
}
void ACLTMonsterBase::SetState(EMonsterState NewState)
{
	CurrentState = NewState;
}

float ACLTMonsterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (CurrentHP <= 0)
	{
		Dead();
		SetState(EMonsterState::Death);
		ACLTMonsterController* AIC = Cast<ACLTMonsterController>(GetController());
		if (AIC)
		{
			AIC->SetState(EMonsterState::Death);
		}

		return DamageAmount;
	}
	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		FPointDamageEvent* Event = (FPointDamageEvent*)(&DamageEvent);
		if (Event)
		{
			CurrentHP -= DamageAmount;
		}

		SpawnHitEffect(Event->HitInfo);
	}
	else
	{
		CurrentHP -= DamageAmount;
	}
	return DamageAmount;
}

