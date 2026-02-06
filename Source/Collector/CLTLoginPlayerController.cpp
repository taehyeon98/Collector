#include "CLTLoginPlayerController.h"
#include "Blueprint/UserWidget.h"

void ACLTLoginPlayerController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = true;
	
	FInputModeUIOnly InputMode;
	SetInputMode(InputMode);
}
