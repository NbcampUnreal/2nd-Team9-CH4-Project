#include "AbilityManager.h"
#include "AbilityManager.h"

#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "GameCore/Ability/TestFireball.h"


UAbilityManager* UAbilityManager::GetInstance()
{
	static UAbilityManager* Instance = NewObject<UAbilityManager>();
	if (Instance && !Instance->IsRooted())
	{
		Instance->AddToRoot();
		//DT
		Instance->AbilityDataTable = TSoftObjectPtr<UDataTable>(FSoftObjectPath(TEXT("/Game/Data/DT_Ability.DT_Ability"))); //경로만 지정됨, 로드x

		//질문
		//지금은 DT가 하나지만 캐릭터 수만큼 늘어날텐데 그럼 결국 캐릭터 직업태그를 인자로 받던가 하고 캐릭터에서 매니저를 생성할텐데
		//캐릭터는 처음부터 모든 스킬 사용가능한데 약참조, 비동기 하는 의미가 있나? -> 어차피 한번에 로드되어야 하지 않나?
		//이 목적이 아닌가???????
		
		FStreamableManager& Streamable = UAssetManager::GetStreamableManager(); //로드 시작
		Streamable.RequestAsyncLoad(
			Instance->AbilityDataTable.ToSoftObjectPath(),
			FStreamableDelegate::CreateUObject(Instance, &UAbilityManager::OnAbilityTableLoaded)); 
	}
	return Instance;
}
void UAbilityManager::Initialize(ACharacter* InOwner)
{
	static const FString ContextString(TEXT("AbilityManager::Initialize"));
	const UDataTable* Table = AbilityDataTable.LoadSynchronous();
	
	TArray<FAbilityRow*> Rows;
	Table->GetAllRows<FAbilityRow>(ContextString, Rows);

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
		FStreamableDelegate::CreateLambda([this, AbilityTag, InOwner, ClassTag]()
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
			Instance->Initialize(InOwner);
		}));
	}
}

// void UAbilityManager::Initialize(ACharacter* InOwner)
// {
// 	//스킬 객체들 생성
// 	static const FString ContextString(TEXT("AbilityManager::Initialize"));
// 	if (AbilityDataTable.ToSoftObjectPath().IsValid())
// 	{
// 		TArray<FAbilityRow*> AbilityRows;
// 		AbilityDataTable->GetAllRows(ContextString, AbilityRows);
// 		for (auto& Ability : AbilityRows)
// 		{
// 			FGameplayTag SkillTag = Ability->AbilityTag;
// 			
// 			FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
// 			
// 			Streamable.RequestAsyncLoad(
// 						Ability->ClassTag.ToSoftObjectPath(),
// 			FStreamableDelegate::CreateLambda
// 					([=,this]()
// 						{
// 							UClass* LoadedClass = Ability->ClassTag.Get();
// 							if (LoadedClass)
// 							{
// 								UAbilityBase* Instance = NewObject<UAbilityBase>(LoadedClass); // 여기 수정 -> 결국은 모든 태그마다 cast 하드코딩 개오반데
// 								AbilityMap.Add(SkillTag, Instance);
// 								Instance->Initialize(InOwner);
// 							}
// 						}
// 					)
// 			);
// 		}
// 			
// 	}
// }

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
}
