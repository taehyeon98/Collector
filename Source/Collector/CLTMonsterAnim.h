// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CLTMonsterBase.h"
#include "CLTMonsterAnim.generated.h"

/**
 * 
 */
UCLASS()
class COLLECTOR_API UCLTMonsterAnim : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = Data)
	float Speed;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = Data)
	EMonsterState CurrentState;
};
