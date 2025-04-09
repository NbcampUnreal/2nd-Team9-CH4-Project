#include "SSBPlayerController.h"
#include "Gameplay/Components/Input/PlayerInputComponent.h"
#include "MessageBus/CheatManager/SSBCheatManager.h"

ASSBPlayerController::ASSBPlayerController()
{
	PlayerInputComponent = CreateDefaultSubobject<UPlayerInputComponent>(TEXT("PlayerInputComponent"));
	CheatClass = USSBCheatManager::StaticClass();
}
