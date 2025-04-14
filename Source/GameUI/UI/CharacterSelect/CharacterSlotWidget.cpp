#include "CharacterSlotWidget.h"

#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "GameFramework/GameStateBase.h"
#include "Gameplay/GameInstance/SSBGameInstance.h"
#include "Gameplay/GameState/CharacterSelect/CharacterSelectGameState.h"
#include "Gameplay/PlayerController/CharacterSelect/CharacterSelectPlayerController.h"

void UCharacterSlotWidget::UpdateWidget(const FCharacterSlotData& CharacterSlotData)
{
	const bool bIsMyWidget = CharacterSlotData.bIsMyWidget;
	OutLineBorder->SetBrushColor(bIsMyWidget ? FLinearColor::Yellow : FLinearColor::Black);
	SelectPrevButton->SetVisibility(bIsMyWidget ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	SelectNextButton->SetVisibility(bIsMyWidget ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

	ReadyBorder->SetVisibility(ESlateVisibility::Hidden);

	SetVisibility(ESlateVisibility::Visible);

	ACharacterSelectPlayerController* CharacterSelectPlayerController
		= Cast<ACharacterSelectPlayerController>(CharacterSlotData.OwnerPlayerController);
	if (bIsMyWidget && IsValid(CharacterSelectPlayerController))
	{
		ReadyButton->SetIsEnabled(CharacterSlotData.PlayerIndex != 0);
		ReadyButton->SetVisibility(bIsMyWidget ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
		ReadyButtonText->SetText(CharacterSlotData.PlayerIndex == 0
			                         ? FText::FromString(TEXT("게임 시작"))
			                         : FText::FromString(TEXT("게임 준비")));

		OwnerController = CharacterSelectPlayerController;
		PlayerIndex = CharacterSlotData.PlayerIndex;
		MaxCharacterTypeIndex = CharacterSlotData.MaxCharacterTypeIndex;
	}
}

bool UCharacterSlotWidget::CanButtonClickAction() const
{
	if (MaxCharacterTypeIndex == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterTypeCount Is Zero"));
		return false;
	}

	if (!IsValid(OwnerController))
	{
		UE_LOG(LogTemp, Warning, TEXT("OwnerController Is Not Valid"));
		return false;
	}

	return true;
}

void UCharacterSlotWidget::HandleSelectButtonClicked(const bool bIsNextButton)
{
	if (!CanButtonClickAction())
	{
		return;
	}

	if (bIsNextButton)
	{
		CharacterTypeIndex++;
	}
	else
	{
		if (CharacterTypeIndex == 0)
		{
			CharacterTypeIndex = MaxCharacterTypeIndex - 1;
		}
		else
		{
			CharacterTypeIndex--;
		}
	}

	CharacterTypeIndex %= MaxCharacterTypeIndex;

	ChangeCharacterModel();
}

void UCharacterSlotWidget::ChangeCharacterModel() const
{
	if (IsValid(OwnerController))
	{
		OwnerController->ServerChangeCharacter(PlayerIndex, CharacterTypeIndex);

		USSBGameInstance* GameInstance = Cast<USSBGameInstance>(GetGameInstance());
		if (IsValid(GameInstance))
		{
			GameInstance->SetCharacterTypeIndex(CharacterTypeIndex);
		}
	}
}

void UCharacterSlotWidget::HandleReadyButtonClicked()
{
	AGameStateBase* GameStateBase = GetWorld()->GetGameState();
	if (IsValid(GameStateBase))
	{
		ACharacterSelectGameState* CharacterSelectGameState = Cast<ACharacterSelectGameState>(GameStateBase);
		if (IsValid(CharacterSelectGameState))
		{
			// Is Host
			if (IsHostWidget())
			{
				CharacterSelectGameState->GoMapSelect();
			}
			// Is Guest
			else
			{
				bIsReady = !bIsReady;
				CharacterSelectGameState->UpdatePlayerReady(PlayerIndex, bIsReady);
			}
		}
	}
}

void UCharacterSlotWidget::UpdateIconTexture(UTexture2D* IconTexture) const
{
	if (IsValid(IconTexture))
	{
		CharacterIconImage->SetBrushFromTexture(IconTexture);
	}
}

void UCharacterSlotWidget::UpdatePlayerReady(const bool bInIsReady)
{
	// Is Host
	if (IsHostWidget())
	{
		ReadyButton->SetIsEnabled(bInIsReady);
	}
	// Is Guest
	else
	{
		bIsReady = bInIsReady;
		ReadyBorder->SetVisibility(bIsReady ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
		ReadyButtonText->SetText(bInIsReady ? FText::FromString(TEXT("준비 취소")) : FText::FromString(TEXT("게임 준비")));
	}
}
