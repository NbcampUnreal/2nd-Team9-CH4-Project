#include "MainMenuMode.h"
#include "GameFramework/PlayerState.h"

void AMainMenuMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	if (GEngine && NewPlayer && NewPlayer->PlayerState)
	{
		const FString PlayerName = NewPlayer->PlayerState->GetPlayerName();
		const FString DebugMessage = FString::Printf(TEXT("Join Player : %s "), *PlayerName);
		
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, DebugMessage);
		UE_LOG(LogTemp, Warning, TEXT("%s"), *DebugMessage);
	}
}
