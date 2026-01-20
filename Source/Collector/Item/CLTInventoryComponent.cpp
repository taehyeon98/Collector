// Fill out your copyright notice in the Description page of Project Settings.


#include "CLTInventoryComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UCLTInventoryComponent::UCLTInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCLTInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
	Inventory.Init(FItemData(), 4);
	for (int32 i = 0; i < Inventory.Num(); i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("���� �ʱ�ȭ [%d] : %s"), i, *Inventory[i].Name.ToString());
	}
}


// Called every frame
void UCLTInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCLTInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCLTInventoryComponent, Inventory);
}

bool UCLTInventoryComponent::AddItem(FName RowName)
{
	UE_LOG(LogTemp, Warning, TEXT("AddItem 실행"));
	if (GetOwner()->HasAuthority())
	{
		if (!ItemDataTable)
		{
			UE_LOG(LogTemp, Warning, TEXT("None Data Table"));
			return false;
		}
		FItemData* OutRow = ItemDataTable->FindRow<FItemData>(RowName, TEXT("Get Item"));
		if (OutRow)
		{
			for (int32 i = 0; i < Inventory.Num(); i++)
			{
				if (Inventory[i].Name == TEXT("Empty"))
				{
					Inventory[i] = *OutRow;
					return true;
				}
			}
		}
		return false;
	}
	else
	{
		C2S_AddItem(RowName);
		return false;
	}
}

void UCLTInventoryComponent::C2S_AddItem_Implementation(FName RowName)
{
	AddItem(RowName);
}

