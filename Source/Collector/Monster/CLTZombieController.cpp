// Fill out your copyright notice in the Description page of Project Settings.


#include "CLTZombieController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "../CLTCharacter.h"
#include "BrainComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "CLTMonsterBase.h"

ACLTZombieController::ACLTZombieController()
{
	UAISenseConfig_Hearing* Hearing = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing"));
	Hearing->HearingRange = 2000.0f;
	Hearing->DetectionByAffiliation.bDetectEnemies = true;
	Hearing->DetectionByAffiliation.bDetectFriendlies = false;
	Hearing->DetectionByAffiliation.bDetectNeutrals = false;
	Perception->ConfigureSense(*Hearing);
	Perception->SetDominantSense(*Hearing->GetSenseImplementation());
}

void ACLTZombieController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (RunBTAsset)
	{
		RunBehaviorTree(RunBTAsset);

	}

	Perception->OnTargetPerceptionForgotten.AddDynamic(this, &ACLTZombieController::ProcessPerceptionForget);
	Perception->OnTargetPerceptionUpdated.AddDynamic(this, &ACLTZombieController::ProcessActorPerception);
	SetGenericTeamId(3);
}

void ACLTZombieController::OnUnPossess()
{
	Super::OnUnPossess();
}

void ACLTZombieController::SetState(EMonsterState NewState)
{
	Super::SetState(NewState);

	Blackboard->SetValueAsEnum(TEXT("State"), (uint8)(NewState));
}

void ACLTZombieController::ProcessPerceptionForget(AActor* Actor)
{
	Super::ProcessPerceptionForget(Actor);

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

void ACLTZombieController::ProcessActorPerception(AActor* Actor, FAIStimulus Stimulus)
{
	Super::ProcessActorPerception(Actor, Stimulus);
	if (Stimulus.Type == UAISense::GetSenseID<UAISense_Hearing>())
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
				Monster->ChangeSpeed(200.0f);
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
}
