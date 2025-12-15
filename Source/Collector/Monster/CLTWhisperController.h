// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CLTMonsterController.h"
#include "CLTWhisperController.generated.h"

/**
 * 
 */
UCLASS()
class COLLECTOR_API ACLTWhisperController : public ACLTMonsterController
{
	GENERATED_BODY()
	
	virtual void ProcessPerceptionForget(AActor* Actor) override;

	virtual void ProcessActorPerception(AActor* Actor, FAIStimulus Stimulus) override;
};
