#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "CharacterSelectGameState.generated.h"

class ACharacterSelectPlayerState;

UCLASS()
class GAMEPLAY_API ACharacterSelectGameState : public AGameState
{
	GENERATED_BODY()

public:
	ACharacterSelectGameState();

	UFUNCTION(Server, Reliable)
	void ServerUpdateCharacterIcon(int32 TargetPlayerIndex, int32 TargetSelectedCharacterIndex);
	
	void NotifyPlayerReadyChanged();

	FORCEINLINE bool IsAllPlayersReady() const { return bIsAllPlayersReady; }
	
private:
	bool AllPlayersReady();

	bool bIsAllPlayersReady;
};
