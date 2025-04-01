#include "SSBPlayerController.h"
#include "Components/Input/PlayerInputComponent.h"

ASSBPlayerController::ASSBPlayerController()
{
	PlayerInputComponent = CreateDefaultSubobject<UPlayerInputComponent>(TEXT("PlayerInputComponent"));
}
