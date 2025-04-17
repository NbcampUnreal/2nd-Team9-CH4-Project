#include "CharacterSelectPlayerState.h"

#include "Gameplay/PlayerController/CharacterSelect/CharacterSelectPlayerController.h"
#include "Net/UnrealNetwork.h"

void ACharacterSelectPlayerState::UpdateReady(const bool bNewReady)
{
	bIsReady = bNewReady;

	const ACharacterSelectPlayerController* PlayerController
		= Cast<ACharacterSelectPlayerController>(GetPlayerController());
	if (IsValid(PlayerController))
	{
		PlayerController->UpdateButtonIsEnabled(bIsReady);
	}
}
