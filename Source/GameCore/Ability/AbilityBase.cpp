#include "AbilityBase.h"

#include "AbilityManager/AbilityManager.h"
#include "Runtime/VerseCompiler/Public/uLang/Parser/VerseGrammar.h"
#include "GameCore/Fighter/Fighter.h"
#include "GameFramework/Character.h"
#include "GameCore/HitBox/HitBox.h"

UAbilityBase::UAbilityBase()
{
	//PrimaryActorTick.bCanEverTick = true;
	OwnerCharacter = nullptr; //임시
}

void UAbilityBase::Initialize()
{
	bIsActive = false;

	if (UAnimInstance* AnimInst = OwnerCharacter->GetMesh()->GetAnimInstance())
	{
		// 한틱차이로 T포즈 애니메이션이 보임 수정해야함
		//AnimInst->OnMontageEnded.AddDynamic(this, &UAbilityBase::OnMontageEnd);

		// 이게맞나? 애님몽타쥬 블렌드아웃 시간을 살짝 준다음 그 사이에 해결..
		//AnimInst->OnMontageBlendingOut.RemoveDynamic(this, &UAbilityBase::OnMontageBlendingOut);
		AnimInst->OnMontageBlendingOut.AddDynamic(this, &UAbilityBase::OnMontageBlendingOut);
		
	}
}

bool UAbilityBase::CanActivate()
{
	if (!OwnerCharacter)
	{
		return false;
	}
	// const FGameplayTag GameplayTag = Cast<AFighter>(OwnerCharacter)->GetGameplayTag(); // 가져오는 건 앉았는지, 좌우이동인지
	// if (BlockedTag.MatchesTag(GameplayTag)) //예를 들어, 내가 앉은 상태에서는 사용 못하는데, 너는 앉아있는 상태니? ->응 
	// {
	// 	return false;	//그럼 돌아가
	// }
	// if (UniqueBlockedTags.HasTag(GameplayTag)) //예를 들어, 내가 "앉은 상태, 뒤로가기" 에서는 사용 못하는데, 너는 이중에 하나라도 포함되니? ->응
	// {
	// 	return false;	//그럼 돌아가
	// }
	// if (UniqueRequiredTags.HasTag(GameplayTag)) //예를 들어, 내가 "idle , 앞으로가기" 에서만 사용 가능한데, 너는 이중에 하나라도 포함되니? ->응
	// {
	// 	return true;	//그럼 진행해
	// }

	return true;
}

void UAbilityBase::Activate(AFighter* Player)
{
	if (!OwnerCharacter)
	{
		OwnerCharacter = Player;
		Initialize();
	}
	
	if(CanActivate()) //태그 체크, Tag check
	{
		PlayMontage(); //몽타주에서 특정 프레임에 이펙트 생성 등등 이벤트
	}
}

void UAbilityBase::PlayMontage()
{
	if (AFighter* Fighter = Cast<AFighter>(OwnerCharacter))
	{
		Fighter->AddAttackTag();
		if (!AirAbilityMontage.IsEmpty() && Fighter->GetGameplayTag().MatchesTag(Fighter->JumpTag))
		{
			int32 Random = FMath::RandRange(0,AirAbilityMontage.Num()-1);
			FString StringName = AirAbilityMontage[Random]->GetName();
			GetWorld()->GetGameInstance()->GetSubsystem<UAbilityManager>()->SetAnimName(FName(*StringName));
			Fighter->PlayAnimMontage(AirAbilityMontage[Random]);
			
			CurrentMontage = AirAbilityMontage[Random];
		}
		else
		{
			int32 Random = FMath::RandRange(0,AbilityMontage.Num()-1);
			FString StringName = AbilityMontage[Random]->GetName();
			GetWorld()->GetGameInstance()->GetSubsystem<UAbilityManager>()->SetAnimName(FName(*StringName));
			Fighter->PlayAnimMontage(AbilityMontage[Random]);
			
			CurrentMontage = AbilityMontage[Random];
		}
	}
}


void UAbilityBase::OnAbilityFinished()
{
	bIsActive = false;
}

void UAbilityBase::OnMontageEnd(UAnimMontage* Montage, bool bInterrupted)
{
	bIsActive = false;
	Cast<AFighter>(OwnerCharacter)->RemoveAttackTag();
	
}

void UAbilityBase::OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted)
{
	bIsActive = false;
	if (GetWorld())
	{
		if (GetWorld()->GetGameInstance())
		{
			if (UAbilityManager* AbilityManager = GetWorld()->GetGameInstance()->GetSubsystem<UAbilityManager>())
			{

				if (!OwnerCharacter->GetGameplayTag().MatchesTag(FGameplayTag::RequestGameplayTag(TEXT("PlayerState.Base.Hit"))))
				{
					/*// 어빌리티 <--> 로코모션 전환을 위한 보정처리
					if (CurrentMontage->GetName().EndsWith(TEXT("stand"), ESearchCase::IgnoreCase))
					{
						OwnerCharacter->SetIdleTag();
					}
					else if (CurrentMontage->GetName().EndsWith(TEXT("crouch"), ESearchCase::IgnoreCase))
					{
						OwnerCharacter->SetGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("PlayerState.Base.Crouch.Idle")));
					}*/
				}
				AbilityManager->AbilityMontageDone();
				//AbilityManager->GetHitBox()->Destroy();
			}
		}
	}
}

bool UAbilityBase::CheckIsPlayingMontage() const
{
	AFighter* Fighter = Cast<AFighter>(OwnerCharacter);
	if (Fighter->GetMesh()->GetAnimInstance()->Montage_IsPlaying(CurrentMontage))
	{
		return true;
	}
	
	return false;
}

FString UAbilityBase::GetMontageName() const
{
	return AbilityMontage[0]->GetName();
}
