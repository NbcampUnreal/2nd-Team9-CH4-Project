#include "CharacterSlotWidget.h"

#include "GameplayTagContainer.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "GameCore/Fighter/CharacterSelect/CharacterSelectPawn.h"
#include "Gameplay/GameInstance/SSBGameInstance.h"
#include "Gameplay/PlayerController/CharacterSelect/CharacterSelectPlayerController.h"
#include "Kismet/KismetSystemLibrary.h"

void UCharacterSlotWidget::InitWidget(const bool bIsMyWidget) const
{
	OutLineBorder->SetBrushColor(bIsMyWidget ? FLinearColor::Red : FLinearColor::Black);
	SelectUpButton->SetVisibility(bIsMyWidget ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	SelectDownButton->SetVisibility(bIsMyWidget ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

bool UCharacterSlotWidget::CanButtonClickAction() const
{
	if (CharacterTypeTagArray.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterTypeTagArray Is Empty"));
		return false;
	}

	if (!IsValid(OwnerController))
	{
		UE_LOG(LogTemp, Warning, TEXT("OwnerController Is Not Valid"));
		return false;
	}

	return true;
}

void UCharacterSlotWidget::HandleButtonClicked(const bool bIsUpButton)
{
	if (!CanButtonClickAction())
	{
		return;
	}

	if (bIsUpButton)
	{
		CharacterTypeTagIndex++;
	}
	else
	{
		if (CharacterTypeTagIndex == 0)
		{
			CharacterTypeTagIndex = CharacterTypeTagArray.Num() - 1;
		}
		else
		{
			CharacterTypeTagIndex--;
		}
	}

	CharacterTypeTagIndex %= CharacterTypeTagArray.Num();

	ChangeCharacterModel();
}

void UCharacterSlotWidget::ChangeCharacterModel()
{
	if (IsValid(OwnerController))
	{
		const FName CharacterTypeTagName = FName(*CharacterTypeTagArray[CharacterTypeTagIndex].ToString());
		OwnerController->ServerChangeCharacter(CharacterTypeTagName);
		
		USSBGameInstance* GameInstance = Cast<USSBGameInstance>(GetGameInstance());
		if (IsValid(GameInstance))
		{
			GameInstance->SetCharacterTypeTag(CharacterTypeTagArray[CharacterTypeTagIndex]);
			UKismetSystemLibrary::PrintString(GetWorld(), CharacterTypeTagName.ToString());
		}
	}
}
