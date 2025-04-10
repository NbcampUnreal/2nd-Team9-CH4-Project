#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "AnimRow.generated.h"

class UAbilityBase;

USTRUCT(BlueprintType)
struct GAMECORE_API FAnimRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitData")
	FName AnimName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitData")
	FName SocketName; //스켈레탈 메시의 어디를 기준으로 스폰될지
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitData")
	FVector3f HitComScale; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitData")
	FName HitComType; //박스, 스피어, 캡슐...

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitData")
	FVector3f HitBoxOffSet; 
};
