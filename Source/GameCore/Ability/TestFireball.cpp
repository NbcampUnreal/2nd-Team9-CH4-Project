#include "TestFireball.h"


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
	return Super::CanActivate();
}

void UTestFireball::Activate()
{
	Super::Activate();
}

void UTestFireball::PlayMontage()
{
	Super::PlayMontage();
}

void UTestFireball::OnAbilityFinished()
{
	Super::OnAbilityFinished();
}

