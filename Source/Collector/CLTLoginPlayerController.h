#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CLTLoginPlayerController.generated.h"

/**
 * PlayerController for the Login Level.
 * Handles mouse cursor and input mode.
 */
UCLASS()
class COLLECTOR_API ACLTLoginPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
};
