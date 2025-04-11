// Fill out your copyright notice in the Description page of Project Settings.


#include "PostEffectComponent.h"
#include "Engine/PostProcessVolume.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/GameplayStatics.h"

UPostEffectComponent::UPostEffectComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPostEffectComponent::BeginPlay()
{
	Super::BeginPlay();

	// 전역 볼륨 생성
	PostProcessVolume = GetWorld()->SpawnActor<APostProcessVolume>();
	if (PostProcessVolume)
	{
		PostProcessVolume->bUnbound = true;
		PostProcessVolume->Settings.bOverride_ColorSaturation = true;
		PostProcessVolume->Settings.ColorSaturation = FVector4(1, 1, 1, 1);

		UMaterialInterface* Mat = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Materials/M_Color.M_Color"));
		if (Mat)
		{
			PostProcessMaterialInstance = UMaterialInstanceDynamic::Create(Mat, this);
			PostProcessVolume->AddOrUpdateBlendable(PostProcessMaterialInstance);
		}
	}
}

void UPostEffectComponent::ApplyGrayscaleEffect()
{
	if (PostProcessVolume)
	{
		PostProcessVolume->Settings.ColorSaturation = FVector4(0, 0, 0, 1);
	}
}

void UPostEffectComponent::SetInversionSphere(FVector Center, float Radius)
{
	if (PostProcessMaterialInstance)
	{
		PostProcessMaterialInstance->SetVectorParameterValue("ColorCenter", FVector4(Center, 1));
		PostProcessMaterialInstance->SetScalarParameterValue("ColorRadius", Radius);
	}
}

