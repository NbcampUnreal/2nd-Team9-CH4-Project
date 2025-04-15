// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_Buffer.h"

#include "GameplayTagContainer.h"
#include "InputActionValue.h"
#include "GameCore/Fighter/Fighter.h"

class AFighter;

void UAnimNotifyState_Buffer::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                          float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	if (MeshComp && MeshComp->GetOwner())
	{
		if (AFighter* Fighter = Cast<AFighter>(MeshComp->GetOwner()))
		{
			Fighter->SetBuffering(true);
		}
	}
}

void UAnimNotifyState_Buffer::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (MeshComp && MeshComp->GetOwner())
	{
		if (AFighter* Fighter = Cast<AFighter>(MeshComp->GetOwner()))
		{
			Fighter->SetBuffering(false);
			const FInputActionValue TestValue;
			Fighter->EndBlocking(TestValue);
		}
	}
}
