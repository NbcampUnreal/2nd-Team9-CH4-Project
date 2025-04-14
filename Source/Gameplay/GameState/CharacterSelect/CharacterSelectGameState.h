#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "CharacterSelectGameState.generated.h"

struct FCharacterSlotData;
class ACharacterSelectPlayerController;

UCLASS()
class GAMEPLAY_API ACharacterSelectGameState : public AGameState
{
	GENERATED_BODY()

public:
	ACharacterSelectGameState();
	
	void InitHUD(ACharacterSelectPlayerController* TargetController);
	
	void UpdatePlayerReady(int32 PlayerIndex, bool bIsReady);

	void GoMapSelect() const;
	
private:
	int32 ReadyCount;
	TArray<bool> PlayerReadyArray;
};
