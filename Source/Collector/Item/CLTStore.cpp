// Fill out your copyright notice in the Description page of Project Settings.


#include "CLTStore.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
ACLTStore::ACLTStore()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StoreZone = CreateDefaultSubobject<UBoxComponent>(TEXT("StoreZone"));

	Store = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Store"));
	Store->SetupAttachment(StoreZone);
}

// Called when the game starts or when spawned
void ACLTStore::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACLTStore::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACLTStore::InStoreZone(APawn* InstigatorPawn)
{
	
}

