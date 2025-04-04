#include "AbilityManager.h"


UAbilityManager* UAbilityManager::GetInstance()
{
	static UAbilityManager* Instance = NewObject<UAbilityManager>();
	if (Instance && !Instance->IsRooted())
	{
		Instance->AddToRoot();
		UObject* LoadedObj = StaticLoadObject(UDataTable::StaticClass(), nullptr, TEXT("/Game/Data/DT_Ability.DT_Ability"));
		if (LoadedObj)
		{
			Instance->AbilityDataTable = Cast<UDataTable>(LoadedObj); //싱글톤 객체를 통해 변수에 접근해야함, 테이블 가져와짐
		}
	}
	return Instance;
}

void UAbilityManager::Initialize(ACharacter* InOwner)
{
	GetAbilityRow();
	for (auto& Ability : Abilities)
	{
		FGameplayTag SkillTag = Ability->AbilityTag;
		UAbilityBase* BPAbility = NewObject<UAbilityBase>(Ability->AbilityClass);
		AbilityMap.Add(SkillTag, BPAbility);
		BPAbility->Initialize(InOwner);
	}
}

void UAbilityManager::RequestCreateAbility(const FGameplayTag& CommandTag)
{
	//요청받은 어빌리티 객체의 Activate() 호출, Activate () call from the requested abilities object

	//컨맨드태그를 직업별스킬태그로 찾아서
	for (auto& Ability : Abilities)
	{
		if (Ability->CommandTag == CommandTag)
		{
			AbilityMap[Ability->AbilityTag]->Activate(); //어빌리티베이스지만 자식어빌리티
		}
		
	}

	
}

void UAbilityManager::GetAbilityRow()
{
	static const FString ContextString(TEXT("AbilityManager::GetAbilityRow"));
	if (AbilityDataTable)
	{
		AbilityDataTable->GetAllRows(ContextString, Abilities);
	}
}
