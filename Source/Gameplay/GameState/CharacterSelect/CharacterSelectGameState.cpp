#include "CharacterSelectGameState.h"

#include "GameFramework/PlayerState.h"
#include "Gameplay/PlayerController/CharacterSelect/CharacterSelectPlayerController.h"
#include "Kismet/KismetSystemLibrary.h"

ACharacterSelectGameState::ACharacterSelectGameState()
{
	ReadyCount = 0;
	PlayerReadyArray.Init(false, 4);
}

void ACharacterSelectGameState::UpdatePlayerReady(const int32 PlayerIndex, const bool bIsReady)
{
	if (PlayerReadyArray.IsValidIndex(PlayerIndex))
	{
		ReadyCount = bIsReady ? ReadyCount + 1 : ReadyCount - 1;

		PlayerReadyArray[PlayerIndex] = bIsReady;

		for (const APlayerState* PlayerState : PlayerArray)
		{
			if (IsValid(PlayerState))
			{
				ACharacterSelectPlayerController* PlayerController
					= Cast<ACharacterSelectPlayerController>(PlayerState->GetPlayerController());
				if (IsValid(PlayerController))
				{
					const bool bIsAllReady = ReadyCount == PlayerArray.Num() - 1;
					PlayerController->ClientUpdatePlayerReady(PlayerIndex, bIsReady, bIsAllReady);
				}
			}
		}
	}
}

void ACharacterSelectGameState::GoMapSelect() const
{
	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Go Map Select!"));
}
