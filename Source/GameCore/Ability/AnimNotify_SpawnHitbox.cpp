
#include "AnimNotify_SpawnHitbox.h"

#include "AbilityData/AnimRow.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "AbilityManager/AbilityManager.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameCore/Fighter/Fighter.h"
#include "GameCore/HitBox/HitBox.h"
#include "GameCore/Ability/Projectile.h"


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
			FAnimRow AnimRow = Manager->GetAnimRow(Manager->GetAnimName());
			//만약에 투사체 날리는 애니메이션이면
			AProjectile* ProjectileInstance = nullptr;
			if(AnimRow.EffectType != EEffectType::Attacked)
			{
				//투사체 생성
				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = MeshComp->GetOwner();
				ProjectileInstance = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, SpawnParams);
				ProjectileInstance->Init(AnimRow);
			}
			
			
			//히트박스 생성
			//현재 스킬의 데미지, 넉백정도 등 가져옴
			FHitDataInfo HitDataInfo = Manager->GetHitDataInfo();
			const FName& AnimName = Manager->GetAnimName();
			
			
			if (AnimRow.BoneName.IsNone())
			{
				UE_LOG(LogTemp, Warning, TEXT("AnimRow is None"));
				return;
			}
			FVector Pos;
			//테이블에서 애님이름으로 row 가져옴 -> 지금 재생되는 몽타주 이름의 소켓이름, 스케일, 타입을 가져옴
			if (Cast<AFighter>(MeshComp->GetOwner())->GetPlayerLookingRight())
			{
				Pos = MeshComp->GetBoneLocation(AnimRow.BoneName);
			}
			else
			{
				FString BoneNameString = AnimRow.BoneName.ToString();
				FString MirroredString;
				if (BoneNameString.EndsWith(TEXT("_r"), ESearchCase::IgnoreCase))
				{
					MirroredString = BoneNameString.LeftChop(2) + TEXT("_l");
				}
				else
				{
					MirroredString = BoneNameString.LeftChop(2) + TEXT("_r");
				}
			
				AnimRow.BoneName = FName(*MirroredString);
				Pos = MeshComp->GetBoneLocation(FName(*MirroredString));
			}
			//오너
			FActorSpawnParameters SpawnParams;
			if (ProjectileInstance)
			{
				SpawnParams.Owner =  ProjectileInstance;
			}
			else
			{
				SpawnParams.Owner = MeshComp->GetOwner();
			}
			
			AHitBox* Instance = GetWorld()->SpawnActor<AHitBox>(HitBoxClass, SpawnParams);

			if (Manager->GetHitBox() != nullptr)
			{
				Manager->GetHitBox()->Destroy();
			}
		
			Manager->SetHitBox(Instance);

			bool bIsMirrored = !Cast<AFighter>(MeshComp->GetOwner())->GetPlayerLookingRight();
			if (Instance)
			{
				HitDataInfo.HitDirection.bIsRight = bIsMirrored;
				Instance->Init(HitDataInfo, Pos , AnimRow);
			}
		}
		
	}
}
