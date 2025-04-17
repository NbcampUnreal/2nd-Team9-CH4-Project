#pragma once

#include "NotifyWidgetInitData.generated.h"

DECLARE_DELEGATE(FOnButtonClicked);

USTRUCT(BlueprintType)
struct FNotifyWidgetText
{
	GENERATED_BODY()

	FNotifyWidgetText()
	{
	}

	FNotifyWidgetText(const FText& NewNotifyText,
					  const FText& NewConfirmButtonText,
					  const FText& NewCancelButtonText)
		: NotifyText(NewNotifyText),
		  ConfirmButtonText(NewConfirmButtonText),
		  CancelButtonText(NewCancelButtonText)
	{
	}

	UPROPERTY(EditDefaultsOnly)
	FText NotifyText;
	UPROPERTY(EditDefaultsOnly)
	FText ConfirmButtonText;
	UPROPERTY(EditDefaultsOnly)
	FText CancelButtonText;
};

USTRUCT()
struct FNotifyWidgetInitData
{
	GENERATED_BODY()

	FNotifyWidgetText NotifyWidgetText;
	
	FOnButtonClicked OnConfirmButtonClickedDelegate;
	FOnButtonClicked OnCancelButtonClickedDelegate;
};