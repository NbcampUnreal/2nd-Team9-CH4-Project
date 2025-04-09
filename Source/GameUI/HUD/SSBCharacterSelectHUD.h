#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SSBCharacterSelectHUD.generated.h"

class UCharacterSelectWidget;

UCLASS(Abstract, Blueprintable)
class GAMEUI_API ASSBCharacterSelectHUD : public AHUD
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

private:

	void CreateCharacterSelectWidget();
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCharacterSelectWidget> CharacterSelectWidgetClass;
	
	TSoftObjectPtr<UCharacterSelectWidget> CharacterSelectWidget;
};
