#include "CharacterSelectHUD.h"

#include "Blueprint/UserWidget.h"
#include "Gameplay/PlayerController/CharacterSelect/CharacterSelectPlayerController.h"
#include "GameUI/UI/CharacterSelect/CharacterSelectWidget.h"

void ACharacterSelectHUD::UpdateCharacterIconTexture(const int32 PlayerIndex, UTexture2D* IconTexture)
{
	CharacterSelectWidget->UpdateCharacterIconTexture(PlayerIndex, IconTexture);
}

void ACharacterSelectHUD::UpdatePlayerReady(const int32 PlayerIndex, const bool bIsReady, const bool bIsAllReady)
{
	CharacterSelectWidget->UpdatePlayerReady(PlayerIndex, bIsReady, bIsAllReady);
}

void ACharacterSelectHUD::BeginPlay()
{
	Super::BeginPlay();

	CreateCharacterSelectWidget();
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
				CharacterSelectWidget->InitWidget();
				CharacterSelectWidget->AddToViewport();
			}
		}
	}
}
