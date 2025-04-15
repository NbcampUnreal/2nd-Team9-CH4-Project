#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameCore/Ability/AbilityBase.h"
#include "GameCore/Ability/CharacterType.h"
#include "GameCore/Ability/AbilityData/AbilityRow.h"
#include "GameCore/Ability/AbilityData/AnimRow.h"
#include "AbilityManager.generated.h"

class AFighter;
class UAbilityManagerHelper;
class AHitBox;
struct FHitDataInfo;

UCLASS(Config = Engine, DefaultConfig, Blueprintable) //Config = Engine, DefaultConfig -> 디폴트.ini, 헬프 클래스
class GAMECORE_API UAbilityManager : public UGameInstanceSubsystem  
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	void InitializeManager(); // 선택된 직업의 어빌리티들 생성, Create the ability of the selected job
	void RequestCreateAbility(const FGameplayTag& CommandTag, bool bIsNext);
	void OnAbilityTableLoaded();
	void OnAnimTableLoaded();
	void UpdateCharacter(ACharacter* InOwner, const ECharacterType InType);

	//Getter, Setter
	const FHitDataInfo& GetHitDataInfo() const;
	const FName& GetAnimName() const;
	AHitBox* GetHitBox() const;
	const FAnimRow* GetAnimRow(const FName& InAnimName) const;
	
	void AbilityMontageDone();

	void SetAnimName(const FName& InAnimName);
	void SetHitBox(AHitBox* InHitBox);

	bool CheckCurrentPlayingMontage() const;
	FString GetNextMontageName() const;
	AFighter* GetPlayerInstance() const { return PlayerInstance.Get(); }
protected:
	UPROPERTY()
	TSoftObjectPtr<UDataTable> AbilityDataTable;

	UPROPERTY()
	TSoftObjectPtr<UDataTable> AnimDataTable;
	
	UPROPERTY()
	TMap<FGameplayTag, UAbilityBase*> AbilityMap; //저장된 태그는 직업별스킬태그 The actual generated Ability object is held and initialized after the use is completed

	UPROPERTY()
	TMap<FGameplayTag, FHitDataInfo> HitInfoMap;
	
	TMap<FGameplayTag, FGameplayTag> CommandTagMap; //<커맨드, 스킬명>

	FHitDataInfo CurrentHitInfo;
	
	TWeakObjectPtr<AFighter> PlayerInstance;

	UPROPERTY(Config)  //Config -> 디폴트.ini, 헬프 클래스
	TSubclassOf<UAbilityManagerHelper> AbilityManagerHelperClass;

	UPROPERTY() //헬프 클래스
	UAbilityManagerHelper* HelperInstance;

	UAbilityBase* CurrentAbility{nullptr};
	UAbilityBase* NextAbility{nullptr};
	FHitDataInfo NextHitInfo;
	
	FName CurrentAnimName;

	UPROPERTY()
	TMap<FName, FAnimRow> AnimInfoMap;
	
	UPROPERTY()
	AHitBox* HitBoxInstance;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterType")
	ECharacterType Type;
};


// 4/9 
/* 
 *왜 서브시스템 상속 받았고 -> 전역 접근이 가능해야 했다. 싱글톤 객체처럼
 *왜 UAbilityManager의 블루프린트로 서브시스템 클래스를 안 만들었는지
 *-> UGameInstanceSubsystem을 상속받은 어빌리티매니저와 BP가 둘 다 생성되기 때문에,
 *-> c++ 객체만 만들고 데이터 테이블을 하드패스로 지정하고 있었다.
 *-> 하지만 하드패스로 지정하는건 지양해야한다.
 *왜 헬프 클래스를 만들었는지? -> bp에서 사용할 데이터 테이블을 등록해두기 위해서. 이렇게 하면 다양한 데이터 테이블들에 쉽게 접근할 수 있다.

DefaultEngine.ini 에 추가한 코드
현재 프로젝트에서 모듈이 나누어져 있고 어빌리티매니저가 위치한 모듈이 GameCore 인 상태
[/Script/GameCore.AbilityManager] : 
AbilityManagerHelperClass=BlueprintGeneratedClass'/Game/Blueprints/GameCore/Ability/BP_Helper.BP_Helper_C'
*/