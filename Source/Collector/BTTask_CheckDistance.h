// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "CLTMonsterBase.h"
#include "BTTask_CheckDistance.generated.h"

UENUM(BlueprintType)
enum class ECondition : uint8
{
	GreaterThan = 0 UMETA(DisplayName = ">"),
	LessThan = 1 UMETA(DisplayName = "<")
};
/**
 * 
 */
UCLASS()
class COLLECTOR_API UBTTask_CheckDistance : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTTask_CheckDistance();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Condition)
	ECondition TargetCondition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Condition)
	EMonsterState TargetState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Condition)
	float TargetDistance;
};
