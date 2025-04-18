﻿#include "SSBGameMode.h"
#include "GameCore/Camera/SSBCamera.h"
#include "GameCore/Fighter/Fighter.h"
#include "Gameplay/PlayerController/SSBPlayerController.h"
#include "GameFramework/PlayerState.h"

void ASSBGameMode::PostLogin(APlayerController* NewPlayer)
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

ASSBGameMode::ASSBGameMode()
{
	PlayerControllerClass = ASSBPlayerController::StaticClass();
	DefaultPawnClass = AFighter::StaticClass();
}
