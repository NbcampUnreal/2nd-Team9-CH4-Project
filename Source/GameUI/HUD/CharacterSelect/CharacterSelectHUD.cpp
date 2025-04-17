#include "CharacterSelectHUD.h"

#include "Blueprint/UserWidget.h"
#include "Gameplay/PlayerController/CharacterSelect/CharacterSelectPlayerController.h"
#include "GameUI/UI/CharacterSelect/CharacterSelectWidget.h"

void ACharacterSelectHUD::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		SetupHUD();
	}
}

void ACharacterSelectHUD::SetupHUD()
{
	ACharacterSelectPlayerController* PlayerController = Cast<ACharacterSelectPlayerController>(GetOwner());
	if (IsValid(PlayerController) && IsValid(CharacterSelectWidgetClass))
    	{
    		CharacterSelectWidget = CreateWidget<UCharacterSelectWidget>(GetWorld(), CharacterSelectWidgetClass);
    		if (IsValid(CharacterSelectWidget))
    		{
    			CharacterSelectWidget->SetupCharacterSelectWidget(PlayerController);
    			CharacterSelectWidget->AddToViewport();
    		}
    	}
}

void ACharacterSelectHUD::ChangedCharacter()
{
	if (IsValid(CharacterSelectWidget))
	{
		CharacterSelectWidget->ChangedCharacter();
	}
}

void ACharacterSelectHUD::UpdateButtonIsEnabled(const bool bIsAllPlayersReady)
{
	if (IsValid(CharacterSelectWidget))
	{
		CharacterSelectWidget->UpdateButtonIsEnabled(bIsAllPlayersReady);
	}
}

void ACharacterSelectHUD::UpdatePlayerReady(const int32 PlayerIndex, const bool bIsReady)
{
	if (IsValid(CharacterSelectWidget))
	{
		CharacterSelectWidget->UpdatePlayerReady(PlayerIndex, bIsReady);
	}
}
