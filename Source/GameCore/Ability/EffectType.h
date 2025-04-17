#pragma once
#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EEffectType : uint8
{
	Attacked     UMETA(DisplayName = "Attacked"),
	Dust     UMETA(DisplayName = "Dust"),
	Gravity      UMETA(DisplayName = "Gravity"),
	Bang		UMETA(DisplayName = "Bang"),
	IceWall		UMETA(DisplayName = "IceWall"),
	PillarOfLight        UMETA(DisplayName = "PillarOfLight"),
	Shield   UMETA(DisplayName = "Shield"),
	Star   UMETA(DisplayName = "Star"),
	Trall   UMETA(DisplayName = "Trall"),
	WindWeather   UMETA(DisplayName = "WindWeather"),
	RainWeather   UMETA(DisplayName = "RainWeather"),
	SnowWeather   UMETA(DisplayName = "SnowWeather"),
	SquallWeather   UMETA(DisplayName = "SquallWeather"),
	EffectEnd   UMETA(DisplayName = "EffectEnd"),
};