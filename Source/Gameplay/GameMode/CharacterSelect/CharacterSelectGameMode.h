#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "CharacterSelectGameMode.generated.h"

UCLASS()
class GAMEPLAY_API ACharacterSelectGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

private:
	int32 CurrentSpawnIndex = 0;
};
