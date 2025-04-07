// Fill out your copyright notice in the Description page of Project Settings.
#include "SSBHUD.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameUI/UI/TitleWidget.h"



void ASSBHUD::BeginPlay()
{
	Super::BeginPlay();
	// Load the UI
 	LoadUI();
}

void ASSBHUD::LoadUI()
{
	if (!GEngine) return;

	UWorld* World = GEngine->GameViewport ? GEngine->GameViewport->GetWorld() : nullptr;
	if (!World) return;

	
	

	if (TitleWidgetClass)
	{
		UTitleWidget* TitleWidget = CreateWidget<UTitleWidget>(World, TitleWidgetClass);
		if (TitleWidget)
		{
			TitleWidget->Init(MainLevel); 
			TitleWidget->AddToViewport();

			// Bind button click events

		}
	}

	else
	{
		UE_LOG(LogTemp, Warning, TEXT("TitleScreenWidget class not found!"));
	}


}

