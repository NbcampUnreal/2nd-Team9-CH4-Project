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
	void TryStartGame() const;
	
private:
	int32 NextPlayerIndex = 0;
};
