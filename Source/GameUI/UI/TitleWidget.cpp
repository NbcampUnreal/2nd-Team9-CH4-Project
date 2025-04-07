// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleWidget.h"
#include "Components/Button.h"
#include <Kismet/GameplayStatics.h>

void UTitleWidget::Init(const TSoftObjectPtr<UWorld> InMainLevel)
{
	
		MainLevel = InMainLevel;
		if (StartButton)
		{
			StartButton->OnClicked.AddDynamic(this, &UTitleWidget::OnStartClicked);
		}
	
	

}

void UTitleWidget::OnStartClicked()
{
	UE_LOG(LogTemp, Log, TEXT("Start button clicked!"));
	// Add your logic to start the game here
	//UGameplayStatics::OpenLevel(GEngine->GetWorld(), FName("MainGameLevel"), true);
	
	    UGameplayStatics::OpenLevelBySoftObjectPtr(this, MainLevel, true);
	
	

}

void UTitleWidget::OnExitClicked()
{
	UE_LOG(LogTemp, Log, TEXT("Exit button clicked!"));
	// Add your logic to exit the game here
	FGenericPlatformMisc::RequestExit(false);
}

