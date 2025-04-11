#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"

#include "CharacterSelectHUD.generated.h"

class UCharacterSelectWidget;

UCLASS(Abstract, Blueprintable)
class GAMEUI_API ACharacterSelectHUD : public AHUD
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	
private:

	UFUNCTION()
	void CreateCharacterSelectWidget();
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCharacterSelectWidget> CharacterSelectWidgetClass;
	
	TSoftObjectPtr<UCharacterSelectWidget> CharacterSelectWidget;
};
