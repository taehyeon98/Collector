// Fill out your copyright notice in the Description page of Project Settings.


#include "CLTStore.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "CLTItemBase.h"
#include "../CLTCharacter.h"
#include "../CLTGameState.h"
#include "TimerManager.h"

// Sets default values
ACLTStore::ACLTStore()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StoreZone = CreateDefaultSubobject<UBoxComponent>(TEXT("StoreZone"));

	StoreMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("StoreMesh"));
	StoreMesh->SetupAttachment(StoreZone);
}

// Called when the game starts or when spawned
void ACLTStore::BeginPlay()
{
	Super::BeginPlay();
	
	StoreZone->OnComponentBeginOverlap.AddDynamic(this, &ACLTStore::OnOverlap);
}

// Called every frame
void ACLTStore::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACLTStore::InStoreZone(APawn* InstigatorPawn)
{
	
}

void ACLTStore::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACLTItemBase* Item = Cast<ACLTItemBase>(OtherActor);
	if (Item)
	{
		FTimerDelegate TimerDel;
		TimerDel.BindUObject(this, &ACLTStore::SellItem, Item);
		
		FTimerHandle Handle;
		GetWorld()->GetTimerManager().SetTimer(Handle, TimerDel, 5.0f, false);
	}
}

void ACLTStore::SellItem(ACLTItemBase* Item)
{
	if (IsValid(Item))
	{
		// Check invalid pointer again just in case destroyed
		ACLTCharacter* Character = Cast<ACLTCharacter>(Item->GetInstigator());
		if (Character)
		{
			// Assuming Coin is in ItemData
			int32 Price = Item->ItemData.Coin;
			
			if (ACLTGameState* GS = GetWorld()->GetGameState<ACLTGameState>())
			{
				GS->SharedGold += Price;
				UE_LOG(LogTemp, Log, TEXT("Sold %s for %d Gold. Shared Gold: %d"), *Item->ItemName.ToString(), Price, GS->SharedGold);
			}
		}
		Item->Destroy();
	}
}

