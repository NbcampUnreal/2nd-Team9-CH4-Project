#include "CharacterSelectWidget.h"

#include "CharacterSlotWidget.h"
#include "Components/HorizontalBox.h"

bool UCharacterSelectWidget::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	for (UWidget* Widget : CharacterSlotBox->GetAllChildren())
	{
		UCharacterSlotWidget* CharacterSlotWidget = Cast<UCharacterSlotWidget>(Widget);
		if (IsValid(CharacterSlotWidget))
		{
			CharacterSlotWidgetArray.Add(CharacterSlotWidget);
		}
	}

	return true;
}

void UCharacterSelectWidget::SetupCharacterSlotWidget(const int32 PlayerIndex)
{
	int32 Index = 0;
	for (UCharacterSlotWidget* CharacterSlotWidget : CharacterSlotWidgetArray)
	{
		if (IsValid(CharacterSlotWidget))
		{
			CharacterSlotWidget->SetupWidget(PlayerIndex == Index, Index == 0);
		}
		Index++;
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

void UCharacterSelectWidget::UpdateReady(const int32 PlayerIndex, const bool bIsReady)
{
	if (CharacterSlotWidgetArray.IsValidIndex(PlayerIndex) && IsValid(CharacterSlotWidgetArray[PlayerIndex]))
	{
		CharacterSlotWidgetArray[PlayerIndex]->UpdateReady(bIsReady);
	}
}
