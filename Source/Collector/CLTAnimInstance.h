// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CLTCharacter.h"
#include "CLTAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class COLLECTOR_API UCLTAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = Data)
	float GroundSpeed = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	uint8 bSprint : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	uint8 bCrouch : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	float Direction;
};
