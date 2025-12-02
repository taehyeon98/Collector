// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CLTMonsterBase.h"
#include "CLTMonsterController.generated.h"


class UAIPerceptionComponent;
class UBehaviorTree;
/**
 * 
 */
UCLASS()
class COLLECTOR_API ACLTMonsterController : public AAIController
{
	GENERATED_BODY()
public:
	ACLTMonsterController();

	virtual void OnPossess(APawn* InPawn) override;

	virtual void OnUnPossess() override;

	void SetState(EMonsterState NewState);

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category = Component)
	TObjectPtr<UAIPerceptionComponent> Perception;
		
	UFUNCTION()
	void ProcessPerceptionForget(AActor* Actor);

	UFUNCTION()
	void ProcessActorPerception(AActor* Actor, FAIStimulus Stimulus);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	TObjectPtr<UBehaviorTree> RunBTAsset;
};
