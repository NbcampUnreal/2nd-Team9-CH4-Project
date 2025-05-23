﻿#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GameplayTagContainer.h" // 대부분 인라인 함수로 구현되어 있고 FGameplayTag, FGameplayTagContainer, FGameplayTagQuery 모두 포함
#include "GameCore/Components/Hit/HitComponent.h"
#include "GameFramework/Actor.h"
#include "AbilityBase.generated.h"


class AFighter;
class UAnimInstance;
class UAnimMontage;
struct FHitDataInfo;

UCLASS(Abstract, Blueprintable)
class GAMECORE_API UAbilityBase : public UObject
{
	GENERATED_BODY()

public:
	UAbilityBase();
	
//게임플레이태그 -> 행동판단 -> 쿼리 이용
//각 행동에서 애니메이션 특정 프레임에 호출할 히트박스 정보도 알아야함 -> 얘기해봐야함
//Gameplay tags-> Determination of behavior-> query
///based actions should also be known to be called hits to call to specific frames->

//내가 생각한 방법, How I thought
//히트박스 위치, 크기 -> 플레이어 본 소켓에 부착, Hitbox position, size-> Attach to the player socket
// 근접(주먹) -> 플레이어 본 소켓에 부착, Melee (Fist) -> Attach to the player socket 
// 칼, 지팡이, 기타 아이템 -> 컴뱃 컴포넌트로 캐릭터에 부착 -> 컴뱃 컴포넌트에 히트박스 부착, Knife, Wand, Other items-> Attach to the character with a combat component-> Hitbox attached to the combat component

//어빌리티, Ability
//실행해도 되는지 상태 체크 + 해당 "캐릭터"의 애니메이션 재생 + 특정 애니메이션 프레임에서 타이밍 조절, 이벤트 발생시킴, Check the status of whether you can run + the timing adjustment in the "character" animation + specific animation frame
public:
	UFUNCTION()
	void Initialize();

	UFUNCTION()
	 bool CanActivate();
	
	UFUNCTION()
	void Activate(AFighter* Player); //어빌리티 사용, Ability use

	UFUNCTION()
	void PlayMontage(); 

	UFUNCTION()
	void OnAbilityFinished(); //어빌리티 사용이 끝나면 사용한 변수들 초기화, Initialization of variables used after the use of abilities
	
	UFUNCTION()
	void OnMontageEnd(UAnimMontage* Montage, bool bInterrupted);
	UFUNCTION()
	void OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted);

	bool CheckIsPlayingMontage() const;

	FString GetMontageName() const;
protected: //잊지말자 소프트레퍼런스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTagContainer UniqueRequiredTags; //각 어빌리티 발동에 필요한 태그들, 아마 플레이어 상태 태그들(지상, 도발, 필살기...),Tags needed for each Ability trigger, maybe player state tags (ground, provocation, killing ...)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTagContainer UniqueBlockedTags;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tag")
	FGameplayTag BlockedTag;
	//태그 컨테이너 세개씩이나 필요없이
	//태그 하나로 지상, 공중, 앉기 조건 걸어서 판단해도 충분할거같다.
	//어빌리티가 절대 발동되지 않는 공통의 태그들(스턴,죽음...) ->이런 애들은 게임스테이트나 다른데서 처리

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	TArray<UAnimMontage*> AbilityMontage; //재생될 애니메이션, Animation to be played
	UAnimMontage* CurrentMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	TArray<UAnimMontage*> AirAbilityMontage; //재생될 애니메이션, Animation to be played
	
	UPROPERTY()
	AFighter* OwnerCharacter;

	UPROPERTY()
	bool bIsActive = false; //필요하다면 사용, Use if necessary

	int32 AirMontageIndex{-1};
};