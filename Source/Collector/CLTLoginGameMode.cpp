#include "CLTLoginGameMode.h"
#include "CLTLoginPlayerController.h"

ACLTLoginGameMode::ACLTLoginGameMode()
{
	PlayerControllerClass = ACLTLoginPlayerController::StaticClass();
	DefaultPawnClass = nullptr; // No pawn needed for login screen
}
