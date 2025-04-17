#pragma once

#include "CoreMinimal.h"
#include "GameCore/Interface/CharacterSelect/CharacterSelectHUDInterface.h"
#include "GameFramework/HUD.h"

#include "CharacterSelectHUD.generated.h"

class UCharacterSelectWidget;

UCLASS(Abstract, Blueprintable)
class GAMEUI_API ACharacterSelectHUD : public AHUD, public ICharacterSelectHUDInterface
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
public:
	// ~ICharacterSelectHUDInterface
	virtual void SetupHUD() override;
	virtual void UpdateCharacterIconTexture(int32 PlayerIndex, UTexture2D* IconTexture) override;
	virtual void UpdateReady(int32 PlayerIndex, bool bIsReady) override;
	// ~End of ICharacterSelectHUDInterface

private:
	UFUNCTION()
	void CreateCharacterSelectWidget();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCharacterSelectWidget> CharacterSelectWidgetClass;

	UPROPERTY()
	TObjectPtr<UCharacterSelectWidget> CharacterSelectWidget;
};
