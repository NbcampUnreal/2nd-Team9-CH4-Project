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

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = MeshComp->GetOwner();
		AHitBox* Instance = GetWorld()->SpawnActor<AHitBox>(HitBoxClass, SpawnParams);
		if (Instance)
		{
			Instance->Init(HitDataInfo);
		}
	}
}

/* 회의 때 말할 거
1. 캐릭터의 어디에 부착할지 추가해야함!!! -> 캐릭터 스켈레탈 메시에 소켓 달고, 구조체에 장착될 소켓 이름 추가하고,
	가져온 구조체에서 소켓이름 과 메시->겟소켓바이네임? 으로 붙이면?

2. 히트 박스 사용 종료 처리 -> 애니메이션 끝나거나 , 오버랩 발생하거나 두가지

*/