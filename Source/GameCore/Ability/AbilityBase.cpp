#include "AbilityBase.h"
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
