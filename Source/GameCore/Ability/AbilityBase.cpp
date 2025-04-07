#include "AbilityBase.h"

#include "GameCore/Fighter/Fighter.h"
#include "GameFramework/Character.h"

UAbilityBase::UAbilityBase()
{
	//PrimaryActorTick.bCanEverTick = true;
	OwnerCharacter = nullptr; //임시
}

void UAbilityBase::Initialize(ACharacter* InOwner)
{
	OwnerCharacter = InOwner;
	bIsActive = false;

	if (UAnimInstance* AnimInst = OwnerCharacter->GetMesh()->GetAnimInstance())
	{
		// 한틱차이로 T포즈 애니메이션이 보임 수정해야함
		//AnimInst->OnMontageEnded.AddDynamic(this, &UAbilityBase::OnMontageEnd);

		// 이게맞나? 애님몽타쥬 블렌드아웃 시간을 살짝 준다음 그 사이에 해결..
		AnimInst->OnMontageBlendingOut.AddDynamic(this, &UAbilityBase::OnMontageBlendingOut);
	}
}

bool UAbilityBase::CanActivate()
{
	return true;
}

void UAbilityBase::Activate()
{
	CanActivate();
	int a= 0;
}

void UAbilityBase::PlayMontage()
{
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
	Cast<AFighter>(OwnerCharacter)->RemoveAttackTag();
}
