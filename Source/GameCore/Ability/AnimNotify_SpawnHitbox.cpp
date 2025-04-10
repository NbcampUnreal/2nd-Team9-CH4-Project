// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_SpawnHitbox.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "AbilityManager/AbilityManager.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameCore/Fighter/Fighter.h"
#include "GameCore/HitBox/HitBox.h"


void UAnimNotify_SpawnHitbox::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	
	//에디터면 리턴해 추가하기!!!!

	if (GIsEditor)
	{
		return;
	}
	
	if (MeshComp && MeshComp->GetOwner())
	{
		if (!GetWorld())
		{
			ensureAlwaysMsgf(false, TEXT("월드가없음")); //한번더검증해
			return;
		}
		if (!GetWorld()->GetGameInstance())
		{
			ensureAlwaysMsgf(false, TEXT("게임인스턴스가없음")); //한번더검증해
			return;
		}
		if (!GetWorld()->GetGameInstance()->GetSubsystem<UAbilityManager>())
		{
			ensureAlwaysMsgf(false, TEXT("매니저가 없음")); //한번더검증해
			return;
		}
		
		//히트박스 생성
		FHitDataInfo HitDataInfo = GetWorld()->GetGameInstance()->GetSubsystem<UAbilityManager>()->GetHitDataInfo();

		//지금 재생되는 몽타주 이름이 무엇인지
		//테이블에서 row 가져옴 -> 지금 재생되는 몽타주 이름의 소켓이름, 스케일, 타입을 가져옴
		//오너
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = MeshComp->GetOwner();
		AHitBox* Instance = GetWorld()->SpawnActor<AHitBox>(HitBoxClass, SpawnParams);
		if (Instance)
		{
			Instance->Init(HitDataInfo);
		}
	}
}
