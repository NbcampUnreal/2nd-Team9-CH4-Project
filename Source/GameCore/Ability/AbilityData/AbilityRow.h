#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"
#include "GameCore/Components/Hit/HitComponent.h"
#include "AbilityRow.generated.h"

class UAbilityBase;

USTRUCT(BlueprintType)
struct GAMECORE_API FAbilityRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	FGameplayTag AbilityTag;  //{파이터.스킬0}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	FGameplayTag CommandTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	TSoftClassPtr<UAbilityBase> ClassTag; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitData")
	FHitDataInfo HitDataInfo;

	
	
};