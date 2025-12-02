// Fill out your copyright notice in the Description page of Project Settings.


#include "CLTPawn.h"

// Sets default values
ACLTPawn::ACLTPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACLTPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACLTPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACLTPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

