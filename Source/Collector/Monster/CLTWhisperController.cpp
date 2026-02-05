// Fill out your copyright notice in the Description page of Project Settings.


#include "CLTWhisperController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "../CLTCharacter.h"
#include "BrainComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "CLTMonsterBase.h"

void ACLTWhisperController::ProcessPerceptionForget(AActor* Actor)
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
		Monster->ChangeSpeed(0);
	}
}

void ACLTWhisperController::ProcessActorPerception(AActor* Actor, FAIStimulus Stimulus)
{
	Super::ProcessActorPerception(Actor, Stimulus);


	if (Stimulus.Type == UAISense::GetSenseID<UAISense_Sight>())
	{
		if (Stimulus.WasSuccessfullySensed())
		{
			ACLTCharacter* Player = Cast<ACLTCharacter>(Actor);
			ACLTMonsterBase* Monster = Cast<ACLTMonsterBase>(GetPawn());
			
			if (Player)
			{
				UE_LOG(LogTemp, Warning, TEXT("Whisper Sensed: %s"), *Player->GetName());
			}

			if (Player && Monster)
			{
				if (Monster->GetCurrentState() == EMonsterState::Death)
				{
					return;
				}
				Blackboard->SetValueAsObject(TEXT("Target"), Player);
				SetState(EMonsterState::Chase);
				Monster->SetState(EMonsterState::Chase);
				Monster->ChangeSpeed(400.0f); // Note: Previous file had 1000.0f, the view showed 400.0f in MonsterController? WhisperController had 1000. Check file content.
			}
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
			Monster->ChangeSpeed(0.0f);
		}
	}
}
