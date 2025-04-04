#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GameCore/Ability/AbilityBase.h"
#include "GameCore/Ability/AbilityData/AbilityRow.h"
#include "AbilityManager.generated.h"

class ABaseAbility;

UCLASS(Blueprintable)
class GAMECORE_API UAbilityManager : public UObject
{
	GENERATED_BODY()

public:
	void Initialize(ACharacter* InOwner); // 선택된 직업의 어빌리티들 생성, Create the ability of the selected job
	void TryUseAbility(const FGameplayTag& CommandTag);
	void GetAbilityRow();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityTags")
	UDataTable* AbilityDataTable;
	
	UPROPERTY()
	TMap<FGameplayTag, UAbilityBase*> AbilityMap; //The actual generated Ability object is held and initialized after the use is completed

	TArray<FAbilityRow*> Abilities;
};
