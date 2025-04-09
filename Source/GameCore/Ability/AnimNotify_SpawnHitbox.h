// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_SpawnHitbox.generated.h"

/**
 * 
 */
class AHitBox;

UCLASS()
class GAMECORE_API UAnimNotify_SpawnHitbox : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HitBox")
	TSubclassOf<AHitBox> HitBoxClass;
};
