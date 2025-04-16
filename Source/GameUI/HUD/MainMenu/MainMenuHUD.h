#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "GameFramework/HUD.h"
#include "MainMenuHUD.generated.h"

class UMainMenuWidget;

UCLASS()
class GAMEUI_API AMainMenuHUD : public AHUD
{
	GENERATED_BODY()
public:
	AMainMenuHUD();
protected:
	virtual void BeginPlay() override;
	virtual void DrawHUD() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UMainMenuWidget> MainMenuWidgetClass;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UMainMenuWidget* MainMenuWidget;
};
