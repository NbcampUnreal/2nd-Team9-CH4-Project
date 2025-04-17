#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "CharacterSelectWidget.generated.h"

class UCharacterSlotWidget;
class UHorizontalBox;

UCLASS(Abstract, Blueprintable)
class GAMEUI_API UCharacterSelectWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual bool Initialize() override;
	
public:
	void SetupCharacterSlotWidget(int32 PlayerIndex);
	void UpdateCharacterIconTexture(int32 InPlayerIndex, UTexture2D* IconTexture);
	void UpdateReady(int32 PlayerIndex, bool bIsReady);

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHorizontalBox> CharacterSlotBox;
	
	TArray<TObjectPtr<UCharacterSlotWidget>> CharacterSlotWidgetArray;
};
