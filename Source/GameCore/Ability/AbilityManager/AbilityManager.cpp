#include "AbilityManager.h"

void UAbilityManager::Initialize(ACharacter* InOwner)
{
	for (auto& Ability : Abilities)
	{
		FGameplayTag SkillTag = Ability->AbilityTag;
		UAbilityBase* BPAbility = NewObject<UAbilityBase>(Ability->AbilityClass);
		AbilityMap[SkillTag] = BPAbility;
	}
}

void UAbilityManager::TryUseAbility(const FGameplayTag& CommandTag)
{
	//요청받은 어빌리티 객체의 Activate() 호출, Activate () call from the requested abilities object
}

void UAbilityManager::GetAbilityRow()
{
	static const FString ContextString(TEXT("AbilityManager::GetAbilityRow"));
	AbilityDataTable->GetAllRows(ContextString, Abilities);
}
