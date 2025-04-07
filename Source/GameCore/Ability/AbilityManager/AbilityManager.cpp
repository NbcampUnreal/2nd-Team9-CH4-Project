#include "AbilityManager.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"

void UAbilityManager::Initialize()
{
	//DT
	AbilityDataTable = TSoftObjectPtr<UDataTable>(FSoftObjectPath(TEXT("/Game/Data/DT_Ability.DT_Ability"))); //경로만 지정됨, 로드x
	FStreamableManager& Streamable = UAssetManager::GetStreamableManager(); //로드 시작
	Streamable.RequestAsyncLoad(
		AbilityDataTable.ToSoftObjectPath(),
		FStreamableDelegate::CreateUObject(this, &UAbilityManager::OnAbilityTableLoaded));
}

void UAbilityManager::RequestCreateAbility(const FGameplayTag& CommandTag)
{
/* 임시로 뭐 들어있는지 확인할려고 작성함 */
#if !UE_BUILD_SHIPPING
	TArray<FGameplayTag> AbilityMapTags;
	AbilityMap.GetKeys(AbilityMapTags);
#endif
	
	//요청받은 어빌리티 객체의 Activate() 호출, Activate () call from the requested abilities object

	//컨맨드태그를 직업별스킬태그로 찾아서
	const FGameplayTag* MappedTag = CommandTagMap.Find(CommandTag);
	if (MappedTag && AbilityMap.Contains(*MappedTag))
	{
		if (UAbilityBase* Ability = AbilityMap[*MappedTag])
		{
			Ability->Activate();
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AbilityMap does not have a key : %s"), *CommandTag.ToString());
	}
}

void UAbilityManager::OnAbilityTableLoaded() //게임 쓰레드에서 실행됨->안전한 객체 생성, 로직처리
{
	//DT 로드가 완료되면 매핑
	static const FString ContextString(TEXT("AbilityManager::OnAbilityTableLoaded"));
	if (AbilityDataTable.IsValid())
	{
		TArray<FAbilityRow*> AbilityRows;
		AbilityDataTable->GetAllRows(ContextString, AbilityRows);

			for (const auto& Ability : AbilityRows)
			{
				FGameplayTag SkillTag = Ability->AbilityTag;
				FGameplayTag CommandTag = Ability->CommandTag;
				CommandTagMap.Add(CommandTag,SkillTag);
			}
	}

	TArray<FAbilityRow*> Rows;
	AbilityDataTable->GetAllRows<FAbilityRow>(ContextString, Rows);
	FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
	
	for (const FAbilityRow* Row : Rows)
	{
		FGameplayTag AbilityTag = Row->AbilityTag;
		TSoftClassPtr<UAbilityBase> ClassTag = Row->ClassTag;

		if (!ClassTag.ToSoftObjectPath().IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("AbilityManager ClassTag is not valid! : %s"), *ClassTag.ToString());
			continue;
		}

		Streamable.RequestAsyncLoad(ClassTag.ToSoftObjectPath(),
		FStreamableDelegate::CreateLambda([this, AbilityTag, ClassTag]()
		{
			UClass* LoadedClass = ClassTag.Get();
			if (!LoadedClass || !LoadedClass->IsChildOf(UAbilityBase::StaticClass()))
			{
				UE_LOG(LogTemp, Warning, TEXT("AbilityManager : Loaded failed : %s"), *AbilityTag.ToString());
				return;
			}

			UAbilityBase* Instance = NewObject<UAbilityBase>(this, LoadedClass);
			if (!Instance)
			{
				UE_LOG(LogTemp, Warning, TEXT("AbilityManager : NewObject failed : %s"), *AbilityTag.ToString());
				return;
			}

			AbilityMap.Add(AbilityTag, Instance);
		}));
	}
}

void UAbilityManager::UpdateCharacter(ACharacter* InOwner) //매핑이 완료되고 각 어빌리티들의 오너를 설정해줌
{
	//OnAbilityTableLoaded() 이 완료되면 캐릭터에게 알림을 보내야함
	//알림 받은 캐릭터거 매지저 호출하면서 this 넘겨줌
	for (auto& it : AbilityMap)
	{
		it.Value->Initialize(InOwner); // player 알아야함, 몽타주아웃이벤트바인딩
	}
}
