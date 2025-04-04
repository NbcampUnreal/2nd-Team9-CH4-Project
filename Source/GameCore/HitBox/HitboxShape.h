#pragma once

#include "CoreMinimal.h"
#include "HitboxShape.generated.h"

UENUM(BlueprintType)
enum class EHitboxShape : uint8
{
	Sphere	UMETA(DisplayName = "Sphere"),
	Capsule	UMETA(DisplayName = "Capsule"),
	Box		UMETA(DisplayName = "Box")
};