// Fill out your copyright notice in the Description page of Project Settings.


#include "CLTInventoryWidget.h"
#include "CLTItemSlotWidget.h"
#include "Components/UniformGridPanel.h"
#include "../Item/CLTInventoryComponent.h"
#include "../CollectorTypes.h"

void UCLTInventoryWidget::Init(UCLTInventoryComponent* InInventoryComponent)
{
	if (InInventoryComponent)
	{
		InventoryComponent = InInventoryComponent;
		InventoryComponent->OnInventoryUpdated.AddDynamic(this, &UCLTInventoryWidget::UpdateInventory);
		UpdateInventory();
	}
}

void UCLTInventoryWidget::UpdateInventory()
{
	if (!InventoryComponent) return;
	if (!ItemGrid) return;
	if (!SlotWidgetClass) return;

	ItemGrid->ClearChildren();
	CurrentSlotIndex = 0;

	const TArray<FItemData>& InventoryItems = InventoryComponent->Inventory;

	for (const FItemData& Item : InventoryItems)
	{
		// Create the slot widget
		UCLTItemSlotWidget* NewSlot = CreateWidget<UCLTItemSlotWidget>(this, SlotWidgetClass);
		if (NewSlot)
		{
			// Add Item even if empty to keep grid structure, or filter if desired. 
			// Assuming we want to show all slots including empty ones:
			NewSlot->InitSlot(FText::FromName(Item.Name), Item.Icon, 1); // Quantity 1 for now

			// Calculate row and column
			int32 Row = CurrentSlotIndex / ColumnsPerRow;
			int32 Column = CurrentSlotIndex % ColumnsPerRow;

			// Add to grid
			ItemGrid->AddChildToUniformGrid(NewSlot, Row, Column);

			CurrentSlotIndex++;
		}
	}
}
