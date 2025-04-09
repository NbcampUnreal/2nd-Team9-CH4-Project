#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameCore/Ability/AbilityBase.h"
#include "GameCore/Ability/AbilityData/AbilityRow.h"
#include "AbilityManager.generated.h"

class AFighter;
struct FHitDataInfo;

UCLASS(Blueprintable)
class GAMECORE_API UAbilityManager : public UGameInstanceSubsystem  
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	void InitializeManager(); // 선택된 직업의 어빌리티들 생성, Create the ability of the selected job
	void RequestCreateAbility(const FGameplayTag& CommandTag); //들어오는건 컨맨드태그
	void OnAbilityTableLoaded();
	void UpdateCharacter(ACharacter* InOwner);
	const FHitDataInfo& GetHitDataInfo() const;

	
protected:
	UPROPERTY()
	TSoftObjectPtr<UDataTable> AbilityDataTable;
	
	UPROPERTY()
	TMap<FGameplayTag, UAbilityBase*> AbilityMap; //저장된 태그는 직업별스킬태그 The actual generated Ability object is held and initialized after the use is completed

	UPROPERTY()
	TMap<FGameplayTag, FHitDataInfo> HitInfoMap;
	
	TMap<FGameplayTag, FGameplayTag> CommandTagMap; //<커맨드, 스킬명>

	FHitDataInfo CurrentHitInfo;
	
	TWeakObjectPtr<AFighter> PlayerInstance;
};
