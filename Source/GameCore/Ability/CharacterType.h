#pragma once
#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ECharacterType : uint8
{
	Fighter     UMETA(DisplayName = "Fighter"),
	Anubis      UMETA(DisplayName = "Anubis"),
	Gunner		UMETA(DisplayName = "Gunner"),
	Mage        UMETA(DisplayName = "Mage"),
	CharacterEnd   UMETA(DisplayName = "CharacterEnd"),
};
