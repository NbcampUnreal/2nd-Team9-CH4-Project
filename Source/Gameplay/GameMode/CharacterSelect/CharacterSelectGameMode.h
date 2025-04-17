#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "CharacterSelectGameMode.generated.h"

UCLASS()
class GAMEPLAY_API ACharacterSelectGameMode : public AGameMode
{
	GENERATED_BODY()

protected:
	virtual void PostLogin(APlayerController* NewPlayer) override;

public:
	void UpdateHostReadyButton() const;
	
	void TryStartGame() const;

	void UpdatePlayerReady(int32 PlayerIndex, bool bIsReady) const;
	
private:
	int32 NextPlayerIndex = 0;
};
