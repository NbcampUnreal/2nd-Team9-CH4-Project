// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_Blocking.h"

#include "GameCore/Fighter/Fighter.h"

void UAnimNotifyState_Blocking::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                            float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	if (MeshComp && MeshComp->GetOwner())
	{
		if (AFighter* Fighter = Cast<AFighter>(MeshComp->GetOwner()))
		{
			Fighter->SetBlocking(true);
		}
	}
}

void UAnimNotifyState_Blocking::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (MeshComp && MeshComp->GetOwner())
	{
		if (AFighter* Fighter = Cast<AFighter>(MeshComp->GetOwner()))
		{
			Fighter->SetBlocking(false);
		}
	}
}
