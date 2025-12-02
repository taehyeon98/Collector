// Fill out your copyright notice in the Description page of Project Settings.


#include "CLTMonsterController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "CLTCharacter.h"
#include "BrainComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "CLTMonsterBase.h"

ACLTMonsterController::ACLTMonsterController()
{
	Perception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception"));

	UAISenseConfig_Sight* Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight"));
	Sight->SightRadius = 600.0f;
	Sight->LoseSightRadius = 700.0f;
	Sight->PeripheralVisionAngleDegrees = 50.0f;
	Sight->DetectionByAffiliation.bDetectEnemies = true;
	Sight->DetectionByAffiliation.bDetectFriendlies = false;
	Sight->DetectionByAffiliation.bDetectNeutrals = false;
	Perception->ConfigureSense(*Sight);
	Perception->SetDominantSense(*Sight->GetSenseImplementation());
}

void ACLTMonsterController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (RunBTAsset)
	{
		RunBehaviorTree(RunBTAsset);

	}

	Perception->OnTargetPerceptionForgotten.AddDynamic(this, &ACLTMonsterController::ProcessPerceptionForget);
	Perception->OnTargetPerceptionUpdated.AddDynamic(this, &ACLTMonsterController::ProcessActorPerception);
	SetGenericTeamId(3);
}

void ACLTMonsterController::OnUnPossess()
{
	Super::OnUnPossess();
}

void ACLTMonsterController::SetState(EMonsterState NewState)
{
	Blackboard->SetValueAsEnum(TEXT("CurrentState"), (uint8)(NewState));
}

void ACLTMonsterController::ProcessPerceptionForget(AActor* Actor)
{
	ACLTCharacter* Player = Cast<ACLTCharacter>(Actor);
	ACLTMonsterBase* Monster = Cast<ACLTMonsterBase>(GetPawn());
	if(Player&&Monster)
	{
		if (Player && Monster)
		{
			if (Monster->GetCurrentState() == EMonsterState::Death)
			{
				return;
			}

			Blackboard->SetValueAsObject(TEXT("Target"), nullptr);
			SetState(EMonsterState::Normal);
			Monster->SetState(EMonsterState::Normal);
			Monster->ChangeSpeed(80.0f);
		}
	}
}

void ACLTMonsterController::ProcessActorPerception(AActor* Actor, FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed())
	{
		ACLTCharacter* Player = Cast<ACLTCharacter>(Actor);
		ACLTMonsterBase* Monster = Cast<ACLTMonsterBase>(GetPawn());
		if (Player && Monster)
		{
			if (Monster->GetCurrentState() == EMonsterState::Death)
			{
				return;
			}

			Blackboard->SetValueAsObject(TEXT("Target"), Player);
			SetState(EMonsterState::Chase);
			Monster->SetState(EMonsterState::Chase);
			Monster->ChangeSpeed(460.0f);
		}
	}
	else
	{
		ACLTCharacter* Player = Cast<ACLTCharacter>(Actor);
		ACLTMonsterBase* Monster = Cast<ACLTMonsterBase>(GetPawn());
		if (Player && Monster)
		{
			if (Monster->GetCurrentState() == EMonsterState::Death)
			{
				return;
			}

			Blackboard->SetValueAsObject(TEXT("Target"), nullptr);
			SetState(EMonsterState::Normal);
			Monster->SetState(EMonsterState::Normal);
			Monster->ChangeSpeed(80.0f);
		}
	}
}
