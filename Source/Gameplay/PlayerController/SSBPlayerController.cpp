#include "SSBPlayerController.h"
#include "Gameplay/Components/Input/PlayerInputComponent.h"

ASSBPlayerController::ASSBPlayerController()
{
	PlayerInputComponent = CreateDefaultSubobject<UPlayerInputComponent>(TEXT("PlayerInputComponent"));
}
