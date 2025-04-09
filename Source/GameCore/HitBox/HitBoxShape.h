#pragma once

#include "CoreMinimal.h"
#include "HitBoxShape.generated.h"

UENUM(BlueprintType)
enum class EHitBoxShape : uint8
{
	Sphere	UMETA(DisplayName = "Sphere"),
	Capsule	UMETA(DisplayName = "Capsule"),
	Box		UMETA(DisplayName = "Box")
};