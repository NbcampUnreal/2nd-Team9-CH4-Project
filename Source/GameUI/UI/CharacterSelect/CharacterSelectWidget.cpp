#include "CharacterSelectWidget.h"

#include "CharacterSlotWidget.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"
#include "Gameplay/PlayerController/CharacterSelect/CharacterSelectPlayerController.h"

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

	bIsHost = false;
	bIsReady = false;

	return true;
}

void UCharacterSelectWidget::SetupCharacterSelectWidget(ACharacterSelectPlayerController* PlayerController)
{
	OwnerPlayerController = PlayerController;
	if (IsValid(OwnerPlayerController))
	{
		const int32 PlayerIndex = OwnerPlayerController->GetPlayerIndex();
		bIsHost = PlayerIndex == 0;
		ReadyButton->SetIsEnabled(!bIsHost);
		ReadyButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnClickedReadyButton);
		ReadyButtonTextBlock->SetText(bIsHost ? FText::FromString(TEXT("Start Game")) : FText::FromString(TEXT("Ready")));

		if (CharacterSlotWidgetArray.IsValidIndex(PlayerIndex))
		{
			MyCharacterSlotWidget = CharacterSlotWidgetArray[PlayerIndex];
			MyCharacterSlotWidget->SetupWidget(this);
		}
	}
}

void UCharacterSelectWidget::OnClickedReadyButton()
{
	if (OwnerPlayerController)
	{
		if (bIsHost)
		{
			OwnerPlayerController->ServerStartGame();
		}
		else
		{
			bIsReady = !bIsReady;
			ReadyButton->SetIsEnabled(false);
			ReadyButtonTextBlock->SetText(bIsReady ? FText::FromString("UnReady") : FText::FromString("Ready"));

			if (IsValid(MyCharacterSlotWidget))
			{
				MyCharacterSlotWidget->ChangeReady(bIsReady);
			}
			
			OwnerPlayerController->ServerUpdateReady(bIsReady);
		}
	}
}

void UCharacterSelectWidget::ChangeCharacter(const bool bIsNext) const
{
	if (IsValid(OwnerPlayerController))
	{
		OwnerPlayerController->ChangeCharacter(bIsNext);
	}
}

void UCharacterSelectWidget::ChangedCharacter() const
{
	if (IsValid(MyCharacterSlotWidget))
	{
		MyCharacterSlotWidget->ChangedCharacter();
	}
}

void UCharacterSelectWidget::UpdateButtonIsEnabled(const bool bIsAllPlayersReady) const
{
	if (bIsHost)
	{
		ReadyButton->SetIsEnabled(bIsAllPlayersReady);
	}
	else
	{
		ReadyButton->SetIsEnabled(true);
	}
}

void UCharacterSelectWidget::UpdatePlayerReady(const int32 PlayerIndex, const bool bTargetIsReady) const
{
	if (CharacterSlotWidgetArray.IsValidIndex(PlayerIndex))
	{
		CharacterSlotWidgetArray[PlayerIndex]->ChangeReady(bTargetIsReady);
	}
}
