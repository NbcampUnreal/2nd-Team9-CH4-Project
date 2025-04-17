#include "CharacterSlotWidget.h"

#include "CharacterSelectWidget.h"
#include "Components/Border.h"
#include "Components/Button.h"

bool UCharacterSlotWidget::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	SetVisibility(ESlateVisibility::Hidden);

	return true;
}

void UCharacterSlotWidget::SetupWidget(UCharacterSelectWidget* NewCharacterSelectWidget)
{
	SelectPrevButton->SetVisibility(ESlateVisibility::Visible);
	SelectNextButton->SetVisibility(ESlateVisibility::Visible);

	SetVisibility(ESlateVisibility::Visible);

	CharacterSelectWidget = NewCharacterSelectWidget;
}

void UCharacterSlotWidget::ChangedCharacter() const
{
	SelectNextButton->SetIsEnabled(true);
	SelectPrevButton->SetIsEnabled(true);
}

void UCharacterSlotWidget::ChangeReady(const bool bIsReady) const
{
	if (IsValid(ReadyTextBorder))
	{
		ReadyTextBorder->SetIsEnabled(bIsReady);
	}
}

void UCharacterSlotWidget::HandleSelectButtonClicked(const bool bIsNext) const
{
	if (!IsValid(CharacterSelectWidget))
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterSelectWidget Is Not Valid"));
		return;
	}

	SelectNextButton->SetIsEnabled(false);
	SelectPrevButton->SetIsEnabled(false);
	
	CharacterSelectWidget->ChangeCharacter(bIsNext);
}
