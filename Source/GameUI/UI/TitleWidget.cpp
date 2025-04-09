// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleWidget.h"
#include "Components/Button.h"
#include <Kismet/GameplayStatics.h>
#include "ModeWidget.h"

void UTitleWidget::Init(const TSoftObjectPtr<UWorld> InMainLevel)
{
	
		MainLevel = InMainLevel;
		if (StartButton)
		{
			StartButton->OnClicked.AddDynamic(this, &UTitleWidget::OnStartClicked);
		}
		if (ExitButton)
		{
			ExitButton->OnClicked.AddDynamic(this, &UTitleWidget::OnExitClicked);
		}
	

}

void UTitleWidget::OnStartClicked()
{
	RemoveFromParent();

	if (ModeWidgetClass)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			UModeWidget* ModeWidget = CreateWidget<UModeWidget>(World, ModeWidgetClass);
			if (ModeWidget)
			{
				ModeWidget->AddToViewport();
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Failed to create ModeWidget!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to get world context!"));
		}
	}
	
	
	    
	    
	
	UE_LOG(LogTemp, Log, TEXT("Start button clicked!"));
}

void UTitleWidget::OnExitClicked()
{
   UE_LOG(LogTemp, Log, TEXT("Exit button clicked!"));
   // Add your logic to exit the game here
   APlayerController* PlayerController = GetWorld()->GetFirstPlayerController(); // Retrieve the player controller
   UKismetSystemLibrary::QuitGame(
       GetWorld(),                          
       PlayerController,                    
       EQuitPreference::Quit,               // 완전히 종료할지, 백그라운드로 보낼지
       true                                 // 플랫폼 제한 무시 (true 권장)
   );
}

