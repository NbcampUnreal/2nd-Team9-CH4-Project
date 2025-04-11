// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PostEffectComponent.generated.h"

class APostProcessVolume;
class UMaterialInstanceDynamic;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAMECORE_API UPostEffectComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPostEffectComponent();

	void ApplyGrayscaleEffect();
	void SetInversionSphere(FVector Center, float Radius);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	APostProcessVolume* PostProcessVolume;

	UPROPERTY()
	UMaterialInstanceDynamic* PostProcessMaterialInstance;

	FVector SphereCenter;
	float SphereRadius;

		
};
