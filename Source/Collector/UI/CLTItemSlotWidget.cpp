// Fill out your copyright notice in the Description page of Project Settings.


#include "CLTItemSlotWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UCLTItemSlotWidget::InitSlot(FText Name, UTexture2D* Icon, int32 Quantity)
{
	if (IconImage)
	{
		if (Icon)
		{
			IconImage->SetBrushFromTexture(Icon);
			IconImage->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			IconImage->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (NameText)
	{
		NameText->SetText(Name);
	}
}
