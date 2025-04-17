#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterSlotWidget.generated.h"


class UBorder;
class UCharacterSelectWidget;
class UButton;

UCLASS(Abstract, Blueprintable)
class GAMEUI_API UCharacterSlotWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual bool Initialize() override;
	
public:
	void SetupWidget(UCharacterSelectWidget* NewCharacterSelectWidget);

	void ChangedCharacter() const;

	void ChangeReady(bool bIsReady) const;
	
private:
	UFUNCTION(BlueprintCallable)
	void HandleSelectButtonClicked(bool bIsNext) const;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> SelectPrevButton;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> SelectNextButton;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> ReadyTextBorder;

	TObjectPtr<UCharacterSelectWidget> CharacterSelectWidget;
};
