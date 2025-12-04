// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CLTMonsterController.h"
#include "CLTMonsterBase.h"
#include "CLTZombieController.generated.h"

/**
 * 
 */
UCLASS()
class COLLECTOR_API ACLTZombieController : public ACLTMonsterController
{
	GENERATED_BODY()
public:
	ACLTZombieController();

	virtual void OnPossess(APawn* InPawn) override;

	virtual void OnUnPossess() override;

	virtual void SetState(EMonsterState NewState) override;

	virtual void ProcessPerceptionForget(AActor* Actor) override;
	
	virtual void ProcessActorPerception(AActor* Actor, FAIStimulus Stimulus) override;
};
