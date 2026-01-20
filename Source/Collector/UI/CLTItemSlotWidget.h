// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CLTItemSlotWidget.generated.h"

class UImage;
class UTextBlock;

/**
 * 
 */
UCLASS()
class COLLECTOR_API UCLTItemSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void InitSlot(FText Name, UTexture2D* Icon, int32 Quantity);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> IconImage;

	// Optional Name text, might not be used if only icon is desired
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> NameText;
};
