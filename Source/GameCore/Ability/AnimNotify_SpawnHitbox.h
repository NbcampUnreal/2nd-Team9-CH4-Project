// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_SpawnHitbox.generated.h"

class AFighter;
/**
 * 
 */
class AHitBox;
class AProjectile;

UCLASS(Blueprintable)
class GAMECORE_API UAnimNotify_SpawnHitbox : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

public:
	void SpawnHitBox(const AFighter* Fighter,const FHitDataInfo& HitDataInfo,const FAnimRow& AnimRow) const;

	UFUNCTION(Server, Reliable)
	void Server_SpawnHitBox(const AFighter* Fighter,const FHitDataInfo& HitDataInfo,const FAnimRow& AnimRow) const;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HitBox")
	TSubclassOf<AHitBox> HitBoxClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AProjectile")
	TSubclassOf<AProjectile> ProjectileClass;
};
