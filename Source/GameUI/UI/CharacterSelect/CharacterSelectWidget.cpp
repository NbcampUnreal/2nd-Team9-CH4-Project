#include "CharacterSelectWidget.h"

#include "CharacterSlotWidget.h"
#include "Components/HorizontalBox.h"
#include "Gameplay/GameInstance/SSBGameInstance.h"
#include "Kismet/KismetSystemLibrary.h"

void UCharacterSelectWidget::InitWidget()
{
	UGameInstance* GameInstance = GetGameInstance();
	if (IsValid(GameInstance))
	{
		const USSBGameInstance* SSBGameInstance = Cast<USSBGameInstance>(GameInstance);
		if (IsValid(SSBGameInstance))
		{
			PlayerIndex = SSBGameInstance->GetPlayerIndex();

			UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Player Index : %d"), PlayerIndex));
		}
	}

	SetCharacterSlotArray();
}

void UCharacterSelectWidget::SetCharacterSlotArray()
{
	if (IsValid(CharacterSlotBox))
	{
		int32 Index = 0;
		for (UWidget* Widget : CharacterSlotBox->GetAllChildren())
		{
			UCharacterSlotWidget* CharacterSlotWidget = Cast<UCharacterSlotWidget>(Widget);
			if (IsValid(CharacterSlotWidget))
			{
				CharacterSlotWidget->InitWidget(PlayerIndex == Index, Index);
				CharacterSlotWidgetArray.Add(CharacterSlotWidget);
				Index++;
			}
		}
	}
}
