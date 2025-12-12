// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_CheckDistance.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Kismet/KismetMathLibrary.h"
#include "../Monster/CLTMonsterBase.h"

UBTTask_CheckDistance::UBTTask_CheckDistance()
{
	NodeName = TEXT("CheckDistance");
}

EBTNodeResult::Type UBTTask_CheckDistance::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AActor* Player = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));
	ACLTMonsterBase* Monster = Cast<ACLTMonsterBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (Player && Monster)
	{
		FVector MonsterLocation = Monster->GetActorLocation();
		FVector PlayerLocation = Player->GetActorLocation();

		float Distance = FVector::Dist2D(MonsterLocation, PlayerLocation);

		switch (TargetCondition)
		{
		case ECondition::GreaterThan:
		{
			if (Distance > TargetDistance)
			{
				Monster->SetState(TargetState);
				OwnerComp.GetBlackboardComponent()->SetValueAsEnum(GetSelectedBlackboardKey(),(uint8)TargetState);
			}
			break;
		}

		case ECondition::LessThan:
		{
			if (Distance < TargetDistance)
			{
				Monster->SetState(TargetState);
				OwnerComp.GetBlackboardComponent()->SetValueAsEnum(GetSelectedBlackboardKey(), (uint8)TargetState);
			}
			break;
		}
		}
	}

	return EBTNodeResult::Type();
}
