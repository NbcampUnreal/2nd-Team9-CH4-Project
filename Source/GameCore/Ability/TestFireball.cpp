#include "TestFireball.h"
#include "GameCore/Fighter/Fighter.h"


UTestFireball::UTestFireball()
{
	//PrimaryActorTick.bCanEverTick = true;
}

void UTestFireball::Initialize(ACharacter* InOwner)
{
	Super::Initialize(InOwner);

}



bool UTestFireball::CanActivate()
{
	//부모에서 1차 검사(스턴, 죽음 등등) 또는 자식에서 부분만족 등 조건 변경, Changes to the first test (stun, death, etc.) in parents or partial satisfaction in children
	if (!OwnerCharacter)
	{
		return false;
	}
	
	const FGameplayTagContainer& PlayerTagContainer = Cast<AFighter>(OwnerCharacter)->GetCurrentTags(); // 혹은 상태별로 가지는 태그, 플레이어에서 함수 만들기, create a function in a tag, a player
	return PlayerTagContainer.HasAny(UnqiueRequiredTags); //어빌리티가 필요한 태그를 가지고 있는지, 필요에 따라서 자식에서 부분만족 등 조건 변경, Whether you have a tag that requires Ability, or changes the conditions for partial satisfaction in your child as needed
	
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
}

void UTestFireball::Activate()
{
	/* 애니메이션 실행 테스트*/
	PlayMontage();
	if (AFighter* Fighter = Cast<AFighter>(OwnerCharacter))
	{
		Fighter->AddAttackTag();
	}
	
	
	if(CanActivate()) //태그 체크, Tag check
	{
		PlayMontage(); //몽타주에서 특정 프레임에 이펙트 생성 등등 이벤트
	}
}

void UTestFireball::PlayMontage()
{
	OwnerCharacter->PlayAnimMontage(AbilityMontage);
}

void UTestFireball::OnAbilityFinished()
{
	Super::OnAbilityFinished();
}

