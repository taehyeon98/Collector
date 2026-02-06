// Fill out your copyright notice in the Description page of Project Settings.


#include "CLTPC.h"

ACLTPC::ACLTPC()
{
}

void ACLTPC::BeginPlay()
{
	Super::BeginPlay();

	SetShowMouseCursor(false);
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
}
