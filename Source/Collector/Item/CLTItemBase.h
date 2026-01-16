// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "../CollectorTypes.h"
#include "CLTItemBase.generated.h"

class UStaticMeshComponent;

UCLASS()
class COLLECTOR_API ACLTItemBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACLTItemBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category = "Component")
	TObjectPtr<class USphereComponent> Sphere;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Component")
	TObjectPtr<class UStaticMeshComponent> Item;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Component")
	FItemData ItemData;
};
