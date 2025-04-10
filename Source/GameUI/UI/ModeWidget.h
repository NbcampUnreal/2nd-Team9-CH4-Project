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
		

protected:
	
	virtual void NativeConstruct() override;
	

private:

	void InitializeGameModes();
	void UpdateDisplayedMode();

	

public:

	//Mode

	UFUNCTION(BlueprintCallable, Category="ModeSelect")
	FGameplayTag ModesSelectTag()const;

	UFUNCTION()
	void ShiftModeRight();

	UFUNCTION()
	void ShiftModeLeft();

	//Time

	UFUNCTION(BlueprintCallable, Category = "ModeSelect")
	FGameplayTag TimesSelectTag()const;

	UFUNCTION(BlueprintCallable, Category = "ModeSelect")
	void ShiftTimeRight();

	UFUNCTION(BlueprintCallable, Category = "ModeSelect")
	void ShiftTimeLeft();
	

private:

	//Mode

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ModeNameText;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* ModeRightButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* ModeLeftButton;

	UPROPERTY(EditAnywhere, Category ="ModeSelect")
	TArray<FGameplayTag> ModeTags;

	int32 CurrentIndex = 1;

	
	// Time

	

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TimeText;

	UPROPERTY(meta = (BindWidget))
	class UButton* TimeRightButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* TimeLeftButton;

	UPROPERTY(EditAnywhere, Category = "ModeSelect")
	TArray<FGameplayTag> TimeTags;

	int32 TimeIndex = 0;

}; 
