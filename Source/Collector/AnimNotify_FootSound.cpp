// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_FootSound.h"
#include "CLTCharacter.h"
#include "Kismet/GamePlayStatics.h"

FString UAnimNotify_FootSound::GetNotifyName_Implementation() const
{
	return TEXT("FootSound");
}

void UAnimNotify_FootSound::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	ACLTCharacter* Char = Cast<ACLTCharacter>(MeshComp->GetOwner());
	if (Char)
	{
		Char->SpawnFootSound();
	}
}
