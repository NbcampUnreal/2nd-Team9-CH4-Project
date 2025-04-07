// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SSBHUD.generated.h"

/**
 * 
 */
UCLASS()
class GAMEUI_API ASSBHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;


private:

	UFUNCTION(BlueprintCallable)
	void LoadUI();

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UTitleWidget> TitleWidgetClass;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UWorld> MainLevel;

};
