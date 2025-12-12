// Fill out your copyright notice in the Description page of Project Settings.


#include "CLTMonsterAnim.h"
#include "GameFramework/CharacterMovementComponent.h"

void UCLTMonsterAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	ACLTMonsterBase* Monster = Cast<ACLTMonsterBase>(TryGetPawnOwner());
	if (Monster)
	{
		Speed = Monster->GetCharacterMovement()->Velocity.Size2D();
		CurrentState = Monster->CurrentState;
	}
}
