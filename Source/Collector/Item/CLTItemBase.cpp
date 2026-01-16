// Fill out your copyright notice in the Description page of Project Settings.


#include "CLTItemBase.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ACLTItemBase::ACLTItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetSphereRadius(80.0f);

	Item = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Item"));
	Item->SetupAttachment(Sphere);
}

// Called when the game starts or when spawned
void ACLTItemBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACLTItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

