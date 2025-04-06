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
	//스킬 객체들 생성
	static const FString ContextString(TEXT("AbilityManager::Initialize"));
	if (AbilityDataTable.IsValid())
	{
		TArray<FAbilityRow*> AbilityRows;
		AbilityDataTable->GetAllRows(ContextString, AbilityRows);
		for (auto& Ability : AbilityRows)
		{
			FGameplayTag SkillTag = Ability->AbilityTag;
			
			FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
			
			Streamable.RequestAsyncLoad(
						Ability->ClassTag.ToSoftObjectPath(),
			FStreamableDelegate::CreateLambda
					([=,this]()
						{
							UClass* LoadedClass = Ability->ClassTag.Get();
							if (LoadedClass)
							{
								UAbilityBase* Instance = NewObject<UAbilityBase>(LoadedClass); // 여기 수정 -> 결국은 모든 태그마다 cast 하드코딩 개오반데
								AbilityMap.Add(SkillTag, Instance);
								Instance->Initialize(InOwner);
							}
						}
					)
			);
		}
			
	}
}

void UAbilityManager::RequestCreateAbility(const FGameplayTag& CommandTag)
{
	//요청받은 어빌리티 객체의 Activate() 호출, Activate () call from the requested abilities object

	//컨맨드태그를 직업별스킬태그로 찾아서

	if (CommandTagMap.Find(CommandTag) != nullptr)
	{
		//애니메이션 재생 확인을 위한 임시코드
		if (UTestFireball* Fireball = Cast<UTestFireball>(AbilityMap[CommandTagMap[CommandTag]]))
		{
			Fireball->Activate();
		}
		//AbilityMap[CommandTagMap[CommandTag]]->Activate(); //어빌리티베이스지만 자식어빌리티
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

			for (auto& Ability : AbilityRows)
			{
				FGameplayTag SkillTag = Ability->AbilityTag;
				FGameplayTag CommandTag = Ability->CommandTag;
				CommandTagMap.Add(CommandTag,SkillTag);
			}
	}
}
