// Fill out your copyright notice in the Description page of Project Settings.


#include "SSBCharacterSelectHUD.h"

#include "Blueprint/UserWidget.h"
#include "GameUI/UI/CharacterSelect/CharacterSelectWidget.h"


void ASSBCharacterSelectHUD::BeginPlay()
{
	Super::BeginPlay();

	CreateCharacterSelectWidget();
}

void ASSBCharacterSelectHUD::CreateCharacterSelectWidget()
{
	if (IsValid(CharacterSelectWidgetClass))
	{
		CharacterSelectWidget = CreateWidget<UCharacterSelectWidget>(GetWorld(), CharacterSelectWidgetClass);
		// CharacterSelectWidget->InitWidget();
		CharacterSelectWidget->AddToViewport();
	}
}
