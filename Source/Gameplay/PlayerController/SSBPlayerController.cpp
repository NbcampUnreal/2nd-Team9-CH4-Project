#include "SSBPlayerController.h"
#include "Input/PlayerInputComponent.h"

ASSBPlayerController::ASSBPlayerController()
{
	PlayerInputComponent = CreateDefaultSubobject<UPlayerInputComponent>(TEXT("PlayerInputComponent"));
}
