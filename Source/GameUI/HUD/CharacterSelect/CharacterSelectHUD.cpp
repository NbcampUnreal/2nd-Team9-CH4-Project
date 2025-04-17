#include "CharacterSelectHUD.h"

#include "Blueprint/UserWidget.h"
#include "Gameplay/PlayerController/CharacterSelect/CharacterSelectPlayerController.h"
#include "GameUI/UI/CharacterSelect/CharacterSelectWidget.h"

void ACharacterSelectHUD::BeginPlay()
{
	Super::BeginPlay();

	SetupHUD();
}

void ACharacterSelectHUD::SetupHUD()
{
	CreateCharacterSelectWidget();
}

void ACharacterSelectHUD::UpdateCharacterIconTexture(const int32 PlayerIndex, UTexture2D* IconTexture)
{
	CharacterSelectWidget->UpdateCharacterIconTexture(PlayerIndex, IconTexture);
}

void ACharacterSelectHUD::UpdateReady(const int32 PlayerIndex, const bool bIsReady)
{
	CharacterSelectWidget->UpdateReady(PlayerIndex, bIsReady);
}

void ACharacterSelectHUD::CreateCharacterSelectWidget()
{
	if (IsValid(CharacterSelectWidgetClass))
	{
		CharacterSelectWidget = CreateWidget<UCharacterSelectWidget>(GetWorld(), CharacterSelectWidgetClass);
		if (IsValid(CharacterSelectWidget))
		{
			ACharacterSelectPlayerController* OwnerPlayerController
				= Cast<ACharacterSelectPlayerController>(GetOwner());
			if (IsValid(OwnerPlayerController))
			{
				const int32 PlayerIndex = OwnerPlayerController->GetPlayerIndex();
				CharacterSelectWidget->SetupCharacterSlotWidget(PlayerIndex);
				CharacterSelectWidget->AddToViewport();
			}
		}
	}
}
