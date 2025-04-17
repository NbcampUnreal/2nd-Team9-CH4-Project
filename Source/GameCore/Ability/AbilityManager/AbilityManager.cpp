#include "AbilityManager.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "GameCore/Ability/AbilityManagerHelper.h"
#include "GameCore/Fighter/Fighter.h"

void UAbilityManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	//InitializeManager();
}

void UAbilityManager::InitializeManager()
{
	if (PlayerInstance.IsValid())
	{
		//DT
		HelperInstance = NewObject<UAbilityManagerHelper>(GetTransientPackage(), AbilityManagerHelperClass); //헬프 클래스, GetTransientPackage -> 게임 시작되면 생성되는 가장 원초적인 객체 , "GC가 안된다"
		FStreamableManager& Streamable = UAssetManager::GetStreamableManager(); //로드 시작
		Streamable.RequestAsyncLoad(
			HelperInstance->AbilityTableArray[static_cast<int32>(Type)].ToSoftObjectPath(),
			FStreamableDelegate::CreateUObject(this, &UAbilityManager::OnAbilityTableLoaded));
	
		Streamable.RequestAsyncLoad(
			HelperInstance->AnimTableArray[static_cast<int32>(Type)].ToSoftObjectPath(),
			FStreamableDelegate::CreateUObject(this, &UAbilityManager::OnAnimTableLoaded));
	}
}

void UAbilityManager::RequestCreateAbility(const FGameplayTag& CommandTag, bool bIsNext)
{
/* 임시로 뭐 들어있는지 확인할려고 작성함 */
#if !UE_BUILD_SHIPPING
	TArray<FGameplayTag> AbilityMapTags;
	AbilityMap.GetKeys(AbilityMapTags);
#endif
	
	//요청받은 어빌리티 객체의 Activate() 호출, Activate () call from the requested abilities object

	if (AbilityMap.Contains(CommandTag))
	{
		if (UAbilityBase* Ability = AbilityMap[CommandTag])
		{
			if (const FHitDataInfo* HitData = HitInfoMap.Find(CommandTag))
			{
				// || (bIsNext && PlayerInstance.Get()->GetCurrentTags().HasTag(PlayerInstance.Get()->AttackTag))
				if (!bIsNext)
				{
					CurrentHitInfo = *HitData; // 사용 끝나면 초기화 시키는 코드 추가해야함!!!!!!!!
					Ability->Activate(PlayerInstance.Get()); //어빌리티 활성화
					CurrentAbility = Ability; 
				}
				else
				{
					NextAbility = Ability;
					NextHitInfo = *HitData;
				}
			}
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
	if (HelperInstance->AbilityTableArray[static_cast<int32>(Type)].IsValid())
	{
		TArray<FAbilityRow*> AbilityRows;
		HelperInstance->AbilityTableArray[static_cast<int32>(Type)]->GetAllRows(ContextString, AbilityRows);

			for (const auto& Ability : AbilityRows)
			{
				FGameplayTag SkillTag = Ability->AbilityTag;
				FGameplayTag CommandTag = Ability->AbilityTag; Ability->CommandTag;
				CommandTagMap.Add(CommandTag,SkillTag);
			}
	}

	TArray<FAbilityRow*> Rows;
	if (HelperInstance->AbilityTableArray[static_cast<int32>(Type)].IsNull())
	{
		ensureAlways(false); //한번더검증해
		return;
	}
	HelperInstance->AbilityTableArray[static_cast<int32>(Type)]->GetAllRows<FAbilityRow>(ContextString, Rows);
	FStreamableManager& Streamable = UAssetManager::GetStreamableManager();

	for (const FAbilityRow* Row : Rows)
	{
		FGameplayTag AbilityTag = Row->AbilityTag;
		FHitDataInfo HitData = Row->HitDataInfo;
		HitInfoMap.Add(AbilityTag, HitData);
		
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

void UAbilityManager::OnAnimTableLoaded()
{
	static const FString ContextString(TEXT("AbilityManager::OnAnimTableLoaded"));
	if (HelperInstance->AnimTableArray[static_cast<int32>(Type)].IsValid())
	{
		TArray<FAnimRow*> AnimRows;
		HelperInstance->AnimTableArray[static_cast<int32>(Type)]->GetAllRows(ContextString, AnimRows);

		for (const auto& Anim : AnimRows)
		{
			FName Name = Anim->AnimName;
			
			FAnimRow AnimInfo;
			AnimInfo.AnimName = Name;
			AnimInfo.BoneName = Anim->BoneName;
			AnimInfo.HitComScale = Anim->HitComScale;
			AnimInfo.HitComType =  Anim->HitComType;
			AnimInfo.Radius = Anim->Radius;
			AnimInfo.HitComOffSet =	Anim->HitComOffSet;
			AnimInfoMap.Add(Name,AnimInfo);
		}
	}
}

void UAbilityManager::UpdateCharacter(ACharacter* InOwner,  const ECharacterType InType) //매핑이 완료되고 각 어빌리티들의 오너를 설정해줌
{
	PlayerInstance = Cast<AFighter>(InOwner);
	Type = InType;
	if (!PlayerInstance.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("AbilityManager : PlayerInstance is not valid!"));
	}
}

const FHitDataInfo& UAbilityManager::GetHitDataInfo() const
{
	return CurrentHitInfo;
}

const FName& UAbilityManager::GetAnimName() const
{
	return CurrentAnimName;
}


void UAbilityManager::SetAnimName(const FName& InAnimName)
{
	CurrentAnimName = InAnimName;
}

AHitBox* UAbilityManager::GetHitBox() const
{
	return HitBoxInstance;
}

void UAbilityManager::SetHitBox(AHitBox* InHitBox)
{
	HitBoxInstance = InHitBox;
}

FAnimRow UAbilityManager::GetAnimRow(const FName& InAnimName)
{
	if (!AnimInfoMap.IsEmpty())
	{
		if (FAnimRow* AnimRow = AnimInfoMap.Find(InAnimName))
		{
			return *AnimRow;	
		}
		else
		{
			for (const auto& InAnimRow : AnimInfoMap)
			{
				UE_LOG(LogTemp, Warning, TEXT("AnimRow Name: %s \t\t AnimRow BoneName : %s"), *(InAnimRow).Value.AnimName.ToString(), *InAnimRow.Value.BoneName.ToString());
			}
			
		}
	}

	return FAnimRow();
}


void UAbilityManager::AbilityMontageDone()
{
	if (NextAbility)
	{
		NextAbility->Activate(PlayerInstance.Get());
		CurrentHitInfo = NextHitInfo;
		CurrentAbility = NextAbility;
		//PlayerInstance.Get()->CurrentMontageName = NextAbility->GetMontageName();
		//PlayerInstance.Get()->RefreshlockTag();
		NextAbility = nullptr;
		NextHitInfo = FHitDataInfo();
	}
	else
	{
		if (CurrentAbility && !CurrentAbility->CheckIsPlayingMontage())
		{
			PlayerInstance.Get()->RemoveAttackTag();
		}
	}
}

bool UAbilityManager::CheckCurrentPlayingMontage() const
{
	if (CurrentAbility)
	{
		return CurrentAbility->CheckIsPlayingMontage();	
	}
	return false;
}

FString UAbilityManager::GetNextMontageName() const
{
	if (NextAbility)
	{
		return NextAbility->GetMontageName();
	}
	else
	{
		return TEXT("");
	}
}
