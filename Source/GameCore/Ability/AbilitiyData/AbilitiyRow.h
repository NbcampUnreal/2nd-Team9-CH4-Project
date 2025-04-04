#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"
#include "AbilitiyRow.generated.h"

class UAbilityBase;

USTRUCT(BlueprintType)
struct FAbilitiyRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	FGameplayTag AbilityTag;  //{파이터.스킬0}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	FGameplayTag CommandTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	TSubclassOf<UAbilityBase> AbilityClass; //수정
};