#include "CharacterSelectWidget.h"

#include "CharacterSlotWidget.h"
#include "Components/HorizontalBox.h"

void UCharacterSelectWidget::InitWidget()
{
	for (UWidget* Widget : CharacterSlotBox->GetAllChildren())
	{
		UCharacterSlotWidget* CharacterSlotWidget
			= Cast<UCharacterSlotWidget>(Widget);

		if (IsValid(CharacterSlotWidget))
		{
			CharacterSlotWidgetArray.Add(CharacterSlotWidget);
		}
	}
}

void UCharacterSelectWidget::UpdateWidget(const FCharacterSlotData& CharacterSlotData)
{
	if (CharacterSlotWidgetArray.IsValidIndex(CharacterSlotData.PlayerIndex))
	{
		UCharacterSlotWidget* CharacterSlotWidget
			= Cast<UCharacterSlotWidget>(CharacterSlotWidgetArray[CharacterSlotData.PlayerIndex]);
		if (IsValid(CharacterSlotWidget))
		{
			CharacterSlotWidget->UpdateWidget(CharacterSlotData);
		}
	}
}

void UCharacterSelectWidget::UpdateCharacterIconTexture(const int32 InPlayerIndex, UTexture2D* IconTexture)
{
	if (CharacterSlotWidgetArray.IsValidIndex(InPlayerIndex))
	{
		if (IsValid(CharacterSlotWidgetArray[InPlayerIndex]))
		{
			CharacterSlotWidgetArray[InPlayerIndex]->UpdateIconTexture(IconTexture);
		}
	}
}

void UCharacterSelectWidget::UpdatePlayerReady(const int32 InPlayerIndex, const bool bIsReady, const bool bIsAllReady)
{
	if (CharacterSlotWidgetArray.IsValidIndex(InPlayerIndex))
	{
		if (IsValid(CharacterSlotWidgetArray[InPlayerIndex]))
		{
			CharacterSlotWidgetArray[InPlayerIndex]->UpdatePlayerReady(bIsReady);

			CharacterSlotWidgetArray[0]->UpdatePlayerReady(bIsAllReady);
		}
	}
}
