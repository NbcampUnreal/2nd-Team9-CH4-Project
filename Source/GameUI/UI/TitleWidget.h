// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TitleWidget.generated.h"

/**
 * 
 */
UCLASS()
class GAMEUI_API UTitleWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	void Init(const TSoftObjectPtr<UWorld> InMainLevel);

private:



	UFUNCTION()
	void OnStartClicked();

	UFUNCTION()
	void OnExitClicked();

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TitleText;

	UPROPERTY(meta = (BindWidget))
	class UButton* StartButton;

	TSoftObjectPtr<UWorld> MainLevel;


};
