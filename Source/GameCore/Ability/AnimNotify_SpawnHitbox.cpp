// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_SpawnHitbox.h"

#include "AbilityData/AnimRow.h"
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
			return;
		}
		if (!GetWorld()->GetGameInstance())
		{
			return;
		}
		if (!GetWorld()->GetGameInstance()->GetSubsystem<UAbilityManager>())
		{
			return;
		}
		UAbilityManager* Manager = GetWorld()->GetGameInstance()->GetSubsystem<UAbilityManager>();
		if (Manager)
		{
			//히트박스 생성
		
			//현재 스킬의 데미지, 넉백정도 등 가져옴
			FHitDataInfo HitDataInfo = Manager->GetHitDataInfo();

			//테이블에서 애님이름으로 row 가져옴 -> 지금 재생되는 몽타주 이름의 소켓이름, 스케일, 타입을 가져옴
			const FAnimRow* AnimRow = Manager->GetAnimRow(Manager->GetAnimName());
			FVector Pos = MeshComp->GetBoneLocation(AnimRow->BoneName);
		
			//오너
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = MeshComp->GetOwner();
			AHitBox* Instance = GetWorld()->SpawnActor<AHitBox>(HitBoxClass, SpawnParams);

			if (Manager->GetHitBox() != nullptr)
			{
				Manager->GetHitBox()->Destroy();
			}
			
			Manager->SetHitBox(Instance);
		
			if (Instance)
			{
				Instance->Init(HitDataInfo, Pos , *AnimRow);
			}
		}
	}
}
