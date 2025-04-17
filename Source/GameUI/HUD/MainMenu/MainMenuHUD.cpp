#include "MainMenuHUD.h"
#include "Blueprint/UserWidget.h"
#include "GameUI/UI/MainMenu/MainMenuWidget.h"

AMainMenuHUD::AMainMenuHUD()
	: MainMenuWidget(nullptr)
{
}

void AMainMenuHUD::BeginPlay()
{
	Super::BeginPlay();
	
	if (MainMenuWidgetClass)
	{
		MainMenuWidget = CreateWidget<UMainMenuWidget>(GetWorld(), MainMenuWidgetClass);
		if (MainMenuWidget)
		{
			MainMenuWidget->AddToViewport();
		}
	}
}

void AMainMenuHUD::DrawHUD()
{
	Super::DrawHUD();
}
