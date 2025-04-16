#include "NotifyWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

bool UNotifyWidget::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	if (IsValid(ConfirmButton))
	{
		ConfirmButton->OnClicked.AddUniqueDynamic(this, &ThisClass::UNotifyWidget::HandleConfirm);
	}

	if (IsValid(CancelButton))
	{
		CancelButton->OnClicked.AddUniqueDynamic(this, &ThisClass::UNotifyWidget::HandleCancel);
	}
	
	return true;
}

void UNotifyWidget::SetupNotifyWidget(const FNotifyWidgetInitData& NotifyWidgetInitData)
{
	InitData = NotifyWidgetInitData;
	
	NotifyTextBlock->SetText(NotifyWidgetInitData.NotifyWidgetText.NotifyText);

	if (NotifyWidgetInitData.NotifyWidgetText.ConfirmButtonText.IsEmptyOrWhitespace())
	{
		ConfirmButton->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		ConfirmButtonTextBlock->SetText(NotifyWidgetInitData.NotifyWidgetText.ConfirmButtonText);
	}
	
	if (NotifyWidgetInitData.NotifyWidgetText.CancelButtonText.IsEmptyOrWhitespace())
	{
		CancelButton->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		CancelButtonTextBlock->SetText(NotifyWidgetInitData.NotifyWidgetText.CancelButtonText);
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UNotifyWidget::HandleConfirm()
{
	if (!InitData.OnConfirmButtonClickedDelegate.ExecuteIfBound())
	{
		UE_LOG(LogTemp, Warning, TEXT("OnConfirmButtonClickedDelegate Is Not Bound"));
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UNotifyWidget::HandleCancel()
{
	if (!InitData.OnCancelButtonClickedDelegate.ExecuteIfBound())
    {
    	UE_LOG(LogTemp, Warning, TEXT("OnCancelButtonClickedDelegate Is Not Bound"));
    }
}
