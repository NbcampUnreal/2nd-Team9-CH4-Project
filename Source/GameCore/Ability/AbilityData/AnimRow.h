#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameCore/Ability/EffectType.h"
#include "AnimRow.generated.h"

USTRUCT(BlueprintType)
struct GAMECORE_API FAnimRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitData")
	FName AnimName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitData")
	FName BoneName; //스켈레탈 메시의 어디를 기준으로 스폰될지
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitData")
	FVector HitComScale; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitData")
	int32 HitComType; //0.스피어, 1.캡슐, 2.박스...

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitData")
	float Radius;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitData")
	FVector HitComOffSet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitData")
	EEffectType EffectType;
};
