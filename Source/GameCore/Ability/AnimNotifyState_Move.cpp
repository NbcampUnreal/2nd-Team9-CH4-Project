// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_Move.h"

#include "GameCore/Fighter/Fighter.h"
#include "GameFramework/PawnMovementComponent.h"

void UAnimNotifyState_Move::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                        float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	if (MeshComp && MeshComp->GetOwner())
	{
		bLookingRight = Cast<AFighter>(MeshComp->GetOwner())->GetPlayerLookingRight();
	}
	if (bLookingRight) // 왼쪽에 서서 오른쪽을 바라보고 있으면
	{
		VecDir = FVector(1.0f, 0, 0.0f);
	}
	else
	{
		VecDir = FVector(-1.0f, 0, 0.0f);
	}
}

void UAnimNotifyState_Move::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                       float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

	if (MeshComp && MeshComp->GetOwner())
	{
		FVector ActorLocation = Cast<AFighter>(MeshComp->GetOwner())->GetActorLocation();
		Cast<AFighter>(MeshComp->GetOwner())->SetActorLocation(ActorLocation+(VecDir * 1000.f * FrameDeltaTime));
		//Cast<AFighter>(MeshComp->GetOwner())->LaunchCharacter(VecDir,true,true);
	}
}
