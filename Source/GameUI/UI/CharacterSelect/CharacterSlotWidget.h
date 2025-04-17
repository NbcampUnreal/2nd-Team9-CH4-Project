#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterSlotWidget.generated.h"

class UTextBlock;
class ACharacterSelectPawn;
class UImage;
class ACharacterSelectPlayerController;
class UBorder;
class UButton;

UCLASS(Abstract, Blueprintable)
class GAMEUI_API UCharacterSlotWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual bool Initialize() override;
	
public:
	void SetupWidget(bool bIsMyWidget, bool bIsHost = false);

	void UpdateIconTexture(UTexture2D* IconTexture) const;
	void UpdateReady(bool bInIsReady);

private:
	UFUNCTION(BlueprintCallable)
	void HandleSelectButtonClicked(bool bIsNextButton);

	UFUNCTION(BlueprintCallable)
	void HandleReadyButtonClicked();

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> OutLineBorder;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> SelectPrevButton;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> SelectNextButton;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> CharacterIconImage;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> ReadyBorder;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ReadyButton;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ReadyButtonText;

	TObjectPtr<ACharacterSelectPlayerController> OwnerController;

	bool bIsHostWidget = false;
	bool bIsReady = false;
};
