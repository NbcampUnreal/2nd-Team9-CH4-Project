// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTagContainer.h"
#include "Engine/DataTable.h"
#include "ModeWidget.generated.h"

/**
 * 
 */


UCLASS()
class GAMEUI_API UModeWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	FGameplayTag UIDataRowTag;
	
protected:

	virtual void NativeConstruct() override;

	UFUNCTION()
	void ButtonClickRight();

	UFUNCTION()
	void ButtonClickLeft();

	UFUNCTION()
	void UpdateText();

	


private:

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ModeNameText;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* RightButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* LeftButton;

	
	int32 CurrentIndex = 0;

};
