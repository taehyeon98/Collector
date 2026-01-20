// Fill out your copyright notice in the Description page of Project Settings.


#include "CLTInventoryWidget.h"
#include "CLTItemSlotWidget.h"
#include "Components/UniformGridPanel.h"

void UCLTInventoryWidget::ClearInventory()
{
	if (ItemGrid)
	{
		ItemGrid->ClearChildren();
	}
	CurrentSlotIndex = 0;
}

void UCLTInventoryWidget::AddItem(FText Name, UTexture2D* Icon, int32 Quantity)
{
	if (!ItemGrid)
	{
		UE_LOG(LogTemp, Warning, TEXT("CLTInventoryWidget: ItemGrid is null. Make sure to bind it in the Widget Blueprint."));
		return;
	}

	if (!SlotWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("CLTInventoryWidget: SlotWidgetClass is not set. Please set it in the Widget Blueprint defaults."));
		return;
	}

	// Create the slot widget
	UCLTItemSlotWidget* NewSlot = CreateWidget<UCLTItemSlotWidget>(this, SlotWidgetClass);
	if (NewSlot)
	{
		NewSlot->InitSlot(Name, Icon, Quantity);

		// Calculate row and column
		int32 Row = CurrentSlotIndex / ColumnsPerRow;
		int32 Column = CurrentSlotIndex % ColumnsPerRow;

		// Add to grid
		ItemGrid->AddChildToUniformGrid(NewSlot, Row, Column);

		CurrentSlotIndex++;
	}
}
