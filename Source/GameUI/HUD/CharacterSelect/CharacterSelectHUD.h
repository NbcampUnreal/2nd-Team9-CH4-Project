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
	virtual void ChangedCharacter() override;
	virtual void UpdateButtonIsEnabled(bool bIsAllPlayersReady) override;
	virtual void UpdatePlayerReady(int32 PlayerIndex, bool bIsReady) override;
	// ~End of ICharacterSelectHUDInterface

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCharacterSelectWidget> CharacterSelectWidgetClass;

	UPROPERTY()
	TObjectPtr<UCharacterSelectWidget> CharacterSelectWidget;
};
