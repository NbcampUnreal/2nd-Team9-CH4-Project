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
	if (!GetWorld() || !GetWorld()->GetGameInstance() || !GetWorld()->GetGameInstance()->GetSubsystem<UAbilityManager>())
	{
		return;
	}
	
	AActor* OwnerActor = MeshComp ? MeshComp->GetOwner() : nullptr;
	AFighter* Fighter = OwnerActor ? Cast<AFighter>(OwnerActor) : nullptr;
	if (!Fighter || (!Fighter->HasAuthority() && !Fighter->IsLocallyControlled()))
	{
		return;
	}
	
	UAbilityManager* Manager = GetWorld()->GetGameInstance()->GetSubsystem<UAbilityManager>();
	
	if (Manager->GetHitBox() != nullptr)
	{
		Manager->GetHitBox()->Destroy();
	}

	FHitDataInfo HitDataInfo = Manager->GetHitDataInfo();
	FAnimRow AnimRow = Manager->GetAnimRow(FName(Animation->GetName()));
	
	bool bIsMirrored = !Fighter->GetPlayerLookingRight();
	HitDataInfo.HitDirection.bIsRight = bIsMirrored;

	if (bIsMirrored)
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
	}

	if (Fighter->HasAuthority())
	{
		SpawnHitBox(Fighter, HitDataInfo, AnimRow);
	}
	else if (Fighter->IsLocallyControlled())
	{
		Server_SpawnHitBox(Fighter, HitDataInfo, AnimRow);
	}
}




void UAnimNotify_SpawnHitbox::SpawnHitBox(const AFighter* Fighter,const FHitDataInfo& HitDataInfo,const FAnimRow& AnimRow) const
{
	const FVector Pos = Fighter->GetMesh()->GetBoneLocation(AnimRow.BoneName);
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Fighter->GetMesh()->GetOwner();
	
	if(AnimRow.EffectType != EEffectType::Attacked)
	{
		AProjectile* ProjectileInstance = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, SpawnParams);
		ProjectileInstance->Init(AnimRow);
		ProjectileInstance->OwnerFighter = Fighter->GetMesh()->GetOwner();
		SpawnParams.Owner = ProjectileInstance;
	}
	
	AHitBox* Instance = GetWorld()->SpawnActor<AHitBox>(HitBoxClass, SpawnParams);
	Instance->Init(HitDataInfo, Pos , AnimRow);
}

void UAnimNotify_SpawnHitbox::Server_SpawnHitBox_Implementation(const AFighter* Fighter,const FHitDataInfo& HitDataInfo,const FAnimRow& AnimRow) const
{
	const FVector Pos = Fighter->GetMesh()->GetBoneLocation(AnimRow.BoneName);
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Fighter->GetMesh()->GetOwner();
	
	if(AnimRow.EffectType != EEffectType::Attacked)
	{
		AProjectile* ProjectileInstance = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, SpawnParams);
		ProjectileInstance->Init(AnimRow);
		ProjectileInstance->OwnerFighter = Fighter->GetMesh()->GetOwner();
		SpawnParams.Owner = ProjectileInstance;
	}
	
	AHitBox* Instance = GetWorld()->SpawnActor<AHitBox>(HitBoxClass, SpawnParams);
	Instance->Init(HitDataInfo, Pos , AnimRow);
}