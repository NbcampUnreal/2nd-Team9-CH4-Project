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

public:
	void InitWidget();
	
private:

	void SetCharacterSlotArray();
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHorizontalBox> CharacterSlotBox;

	TArray<TObjectPtr<UCharacterSlotWidget>> CharacterSlotWidgetArray;

	int32 PlayerIndex = 0;
};
