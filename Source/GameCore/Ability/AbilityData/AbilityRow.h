#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"
#include "AbilityRow.generated.h"

class UAbilityBase;

USTRUCT(BlueprintType)
struct FAbilityRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	FGameplayTag AbilityTag;  //{파이터.스킬0}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	FGameplayTag CommandTag;

	/*
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	TSubclassOf<UAbilityBase> AbilityClass; 강참조
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	TSoftClassPtr<UAbilityBase> ClassTag; // 약참조로 바꿈 -> 데이터 테이블에 직접 클래스 경로 넣어줄 필요있음
};