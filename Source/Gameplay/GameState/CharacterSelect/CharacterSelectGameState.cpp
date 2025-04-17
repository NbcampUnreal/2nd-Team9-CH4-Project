#include "CharacterSelectGameState.h"

#include "Gameplay/PlayerController/CharacterSelect/CharacterSelectPlayerController.h"
#include "Gameplay/PlayerState/CharacterSelectPlayerState.h"


ACharacterSelectGameState::ACharacterSelectGameState()
{
	bIsAllPlayersReady = false;
}

void ACharacterSelectGameState::UpdateHostStartButtonIsEnabled()
{
	bIsAllPlayersReady = AllPlayersReady();
	ACharacterSelectPlayerState* CharacterSelectPlayerState = Cast<ACharacterSelectPlayerState>(PlayerArray[0]);
	if (IsValid(CharacterSelectPlayerState))
	{
		CharacterSelectPlayerState->UpdateReady(bIsAllPlayersReady);
	}
}

bool ACharacterSelectGameState::AllPlayersReady()
{
	for (int32 Index = 1; Index < PlayerArray.Num(); Index++)
	{
		const ACharacterSelectPlayerState* CharacterSelectPlayerState = Cast<ACharacterSelectPlayerState>(PlayerArray[Index]);
		if (IsValid(CharacterSelectPlayerState) && !CharacterSelectPlayerState->IsReady())
		{
			return false;
		}
	}
	return true;
}
