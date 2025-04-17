#include "CharacterSlotWidget.h"

#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Gameplay/PlayerController/CharacterSelect/CharacterSelectPlayerController.h"

bool UCharacterSlotWidget::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	SetVisibility(ESlateVisibility::Hidden);

	return true;
}

void UCharacterSlotWidget::SetupWidget(const bool bIsMyWidget, const bool bIsHost)
{
	OutLineBorder->SetBrushColor(bIsMyWidget ? FLinearColor::Yellow : FLinearColor::Black);
	SelectPrevButton->SetVisibility(bIsMyWidget ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	SelectNextButton->SetVisibility(bIsMyWidget ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

	ReadyBorder->SetVisibility(ESlateVisibility::Hidden);

	SetVisibility(bIsMyWidget ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	
	if (bIsMyWidget)
	{
		bIsHostWidget = bIsHost;

		ReadyButton->SetIsEnabled(!bIsHost);
		ReadyButton->SetVisibility(bIsMyWidget ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
		ReadyButtonText->SetText(bIsHost ? FText::FromString(TEXT("Game Start")) : FText::FromString(TEXT("Ready")));

		ACharacterSelectPlayerController* CharacterSelectPlayerController
			= Cast<ACharacterSelectPlayerController>(GetOwningPlayer());
		if (IsValid(CharacterSelectPlayerController))
		{
			OwnerController = CharacterSelectPlayerController;
		}
	}
}

void UCharacterSlotWidget::HandleSelectButtonClicked(const bool bIsNextButton)
{
	if (!IsValid(OwnerController))
	{
		UE_LOG(LogTemp, Warning, TEXT("OwnerController Is Not Valid"));
		return;
	}

	if (IsValid(OwnerController))
	{
		SelectNextButton->SetIsEnabled(false);
		SelectPrevButton->SetIsEnabled(false);
		OwnerController->ServerChangeCharacter(bIsNextButton);
	}
}

void UCharacterSlotWidget::HandleReadyButtonClicked()
{
	if (IsValid(OwnerController))
	{
		// Is Host
		if (bIsHostWidget)
		{
			OwnerController->ServerStartGame();
		}
		// Is Guest
		else
		{
			ReadyButton->SetIsEnabled(false);
			ReadyButtonText->SetText(bIsReady ? FText::FromString("Unready") : FText::FromString("Ready"));

			OwnerController->ServerUpdateReady(!bIsReady);
		}
	}
}

void UCharacterSlotWidget::UpdateIconTexture(UTexture2D* IconTexture) const
{
	if (IsValid(IconTexture))
	{
		CharacterIconImage->SetBrushFromTexture(IconTexture);
		SelectNextButton->SetIsEnabled(true);
		SelectPrevButton->SetIsEnabled(true);
	}
}

void UCharacterSlotWidget::UpdateReady(const bool bInIsReady)
{
	// Is Host
	if (bIsHostWidget)
	{
		ReadyButton->SetIsEnabled(bInIsReady);
	}
	// Is Guest
	else
	{
		bIsReady = bInIsReady;

		ReadyBorder->SetVisibility(bIsReady ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
		
		ReadyButton->SetIsEnabled(true);
		ReadyButtonText->SetText(bInIsReady ? FText::FromString(TEXT("준비 취소")) : FText::FromString(TEXT("게임 준비")));
	}
}
