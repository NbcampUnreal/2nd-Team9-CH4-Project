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
	static UAbilityManager* GetInstance();
	

	void Initialize(ACharacter* InOwner); // 선택된 직업의 어빌리티들 생성, Create the ability of the selected job
	void RequestCreateAbility(const FGameplayTag& CommandTag); //들어오는건 컨맨드태그
	void GetAbilityRow();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityTags")
	UDataTable* AbilityDataTable;
	
	UPROPERTY()
	TMap<FGameplayTag, UAbilityBase*> AbilityMap; //저장된 태그는 직업별스킬태그 The actual generated Ability object is held and initialized after the use is completed

	TArray<FAbilityRow*> Abilities;

	//*****생성된 객체들 가비지컬렉션 되지 않는가 체크, 어떻게 관리하는가
};
