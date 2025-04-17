#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "CharacterSelectHUDInterface.generated.h"

UINTERFACE()
class GAMECORE_API UCharacterSelectHUDInterface : public UInterface
{
	GENERATED_BODY()
};

class GAMECORE_API ICharacterSelectHUDInterface
{
	GENERATED_BODY()

public:
	virtual void SetupHUD() = 0;
	virtual void ChangedCharacter() = 0;
	virtual void UpdateButtonIsEnabled(bool bIsAllPlayersReady) = 0;
	virtual void UpdatePlayerReady(int32 PlayerIndex, bool bIsReady) = 0;
};
