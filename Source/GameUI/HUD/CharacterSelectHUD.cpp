// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSelectHUD.h"

#include "Blueprint/UserWidget.h"
#include "GameUI/UI/CharacterSelect/CharacterSelectWidget.h"


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
		CharacterSelectWidget->InitWidget();
		CharacterSelectWidget->AddToViewport();
	}
}
