// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../CollectorTypes.h"
#include "Engine/DataTable.h"
#include "CLTInventoryComponent.generated.h"


UCLASS(Blueprintable,meta = (BlueprintSpawnableComponent))
class COLLECTOR_API UCLTInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCLTInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Data")
	UDataTable* ItemDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", Replicated)
	TArray<FItemData> Inventory;

	UFUNCTION(BlueprintCallable)
	bool AddItem(FName RowName);

	UFUNCTION(BlueprintCallable)
	FName RemoveItem(int32 SlotIndex);

	UFUNCTION(Server, Reliable)
	void C2S_AddItem(FName RowName);
	void C2S_AddItem_Implementation(FName RowName);
};
