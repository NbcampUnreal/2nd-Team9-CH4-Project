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
	
	void UpdateHostStartButtonIsEnabled();

	FORCEINLINE bool IsAllPlayersReady() const { return bIsAllPlayersReady; }
	
private:
	bool AllPlayersReady();

	bool bIsAllPlayersReady;
};
