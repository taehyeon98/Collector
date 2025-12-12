// Fill out your copyright notice in the Description page of Project Settings.


#include "CLTAnimInstance.h"
#include "../CLTCharacter.h"
#include "Gameframework/CharacterMovementComponent.h"
#include "KismetAnimationLibrary.h"

void UCLTAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	ACLTCharacter* Character = Cast<ACLTCharacter>(TryGetPawnOwner());
	if (Character)
	{
		GroundSpeed = Character->GetCharacterMovement()->Velocity.Size2D();
		Direction = UKismetAnimationLibrary::CalculateDirection(Character->GetCharacterMovement()->Velocity, Character->GetActorRotation());
		bCrouch = Character->bIsCrouched;
		bSprint = Character->bSprint;
	}
}
