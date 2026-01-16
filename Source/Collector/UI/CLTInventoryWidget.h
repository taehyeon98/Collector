// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CLTInventoryWidget.generated.h"

class UUniformGridPanel;
class UCLTItemSlotWidget;

/**
 * 
 */
UCLASS()
class COLLECTOR_API UCLTInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void ClearInventory();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddItem(FText Name, UTexture2D* Icon, int32 Quantity);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> ItemGrid;

	// The widget class to use for individual slots
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TSubclassOf<UCLTItemSlotWidget> SlotWidgetClass;

private:
	int32 CurrentSlotIndex = 0;
    
    // Configurable columns count
    UPROPERTY(EditDefaultsOnly, Category = "Inventory")
    int32 ColumnsPerRow = 4;
};
