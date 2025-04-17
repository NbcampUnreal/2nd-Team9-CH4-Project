#include "CharacterSelectGameMode.h"

#include "Gameplay/GameInstance/Subsystem/EOSSessionSubsystem.h"
#include "Gameplay/GameState/CharacterSelect/CharacterSelectGameState.h"
#include "Gameplay/PlayerController/CharacterSelect/CharacterSelectPlayerController.h"
#include "Kismet/KismetSystemLibrary.h"

void ACharacterSelectGameMode::PostLogin(APlayerController* NewPlayer)
{
	ACharacterSelectPlayerController* PlayerController = Cast<ACharacterSelectPlayerController>(NewPlayer);
	if (IsValid(PlayerController))
	{
		// Is Not Host
		if (NextPlayerIndex > 0)
		{
			UpdateHostReadyButton();
		}
		
		UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Post Login : %d"), NextPlayerIndex));
		PlayerController->SetPlayerIndex(NextPlayerIndex++);
	}
	
	Super::PostLogin(NewPlayer);
}

void ACharacterSelectGameMode::UpdateHostReadyButton() const
{
	ACharacterSelectPlayerController* HostPlayerController
	= Cast<ACharacterSelectPlayerController>(GetWorld()->GetFirstPlayerController());
	if (IsValid(HostPlayerController))
	{
		HostPlayerController->ClientUpdatePlayerReady(0, false);
	}
}

void ACharacterSelectGameMode::TryStartGame() const
{
	// Check All Players Ready
	const ACharacterSelectGameState* CharacterSelectGameState = GetGameState<ACharacterSelectGameState>();
	if (IsValid(CharacterSelectGameState) && CharacterSelectGameState->IsAllPlayersReady())
	{
		UEOSSessionSubsystem* SessionSubsystem = GetGameInstance()->GetSubsystem<UEOSSessionSubsystem>();
		if (IsValid(SessionSubsystem))
		{
			SessionSubsystem->StartSession();
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Is Not All Players Ready"));
	}
}

void ACharacterSelectGameMode::UpdatePlayerReady(const int32 PlayerIndex, const bool bIsReady) const
{
	for (auto It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		ACharacterSelectPlayerController* PlayerController = Cast<ACharacterSelectPlayerController>(*It);
		if (IsValid(PlayerController) && PlayerController->GetPlayerIndex() != PlayerIndex)
		{
			PlayerController->ClientUpdatePlayerReady(PlayerIndex, bIsReady);
		}
	}
}
