#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GameCore/Ability/AbilityBase.h"
#include "GameCore/Ability/AbilityData/AbilityRow.h"
#include "AbilityManager.generated.h"

UCLASS(Blueprintable)
class GAMECORE_API UAbilityManager : public UObject  
{
	GENERATED_BODY()
	
public:
	static UAbilityManager* GetInstance();
	

	void Initialize(ACharacter* InOwner); // 선택된 직업의 어빌리티들 생성, Create the ability of the selected job
	void RequestCreateAbility(const FGameplayTag& CommandTag); //들어오는건 컨맨드태그
	void OnAbilityTableLoaded();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityTags")
	TSoftObjectPtr<UDataTable> AbilityDataTable;
	
	UPROPERTY()
	TMap<FGameplayTag, UAbilityBase*> AbilityMap; //저장된 태그는 직업별스킬태그 The actual generated Ability object is held and initialized after the use is completed

	TMap<FGameplayTag, FGameplayTag> CommandTagMap;

	//*****생성된 객체들 가비지컬렉션 되지 않는가 체크, 어떻게 관리하는가

	//캐릭터 생성시(캐릭터 선택 레벨, 이때 캐릭터는 그냥 텍스처?메시?와 ID만 있는 간단한 다른 클래스?..에서 생성 후 다음 레벨이 로드되면)
	//스킬 객체들 생성하되 -> 게임 시작되면 필살기 제외한 모든 스킬이 즉시 사용 가능해야함
	//필살기 어빌리티 객체는 비동기로 생성, 필살기 게이지가 찰 동안 시간적 여유가 있으니까
};
