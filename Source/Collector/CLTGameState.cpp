// Fill out your copyright notice in the Description page of Project Settings.


#include "CLTGameState.h"
#include "Net/UnrealNetwork.h"

void ACLTGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACLTGameState, SharedGold);
}
