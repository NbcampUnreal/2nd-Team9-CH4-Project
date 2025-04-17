#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "CharacterSelectWidget.generated.h"

class UTextBlock;
class UButton;
class ACharacterSelectPlayerController;
class UCharacterSlotWidget;
class UHorizontalBox;

UCLASS(Abstract, Blueprintable)
class GAMEUI_API UCharacterSelectWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual bool Initialize() override;
	
public:
	UFUNCTION()
	void OnClickedReadyButton();
	
	void SetupCharacterSelectWidget(ACharacterSelectPlayerController* PlayerController);
	void ChangeCharacter(bool bIsNext) const;
	void ChangedCharacter() const;

	void UpdateButtonIsEnabled(bool bIsAllPlayersReady) const;

	void UpdatePlayerReady(int32 PlayerIndex, bool bTargetIsReady) const;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHorizontalBox> CharacterSlotBox;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ReadyButton;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ReadyButtonTextBlock;
	
	TArray<TObjectPtr<UCharacterSlotWidget>> CharacterSlotWidgetArray;
	TObjectPtr<UCharacterSlotWidget> MyCharacterSlotWidget;

	TObjectPtr<ACharacterSelectPlayerController> OwnerPlayerController;

	bool bIsHost;
	bool bIsReady;
};
