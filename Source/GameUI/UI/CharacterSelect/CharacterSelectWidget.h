#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "CharacterSelectWidget.generated.h"

struct FCharacterSlotData;
class UCharacterSlotWidget;
class UHorizontalBox;

UCLASS(Abstract, Blueprintable)
class GAMEUI_API UCharacterSelectWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	void InitWidget();
	
	void UpdateWidget(const FCharacterSlotData& CharacterSlotData);
	
	void UpdateCharacterIconTexture(int32 InPlayerIndex, UTexture2D* IconTexture);
	void UpdatePlayerReady(int32 InPlayerIndex, bool bIsReady, bool bIsAllReady);

private:
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHorizontalBox> CharacterSlotBox;

	TArray<TObjectPtr<UCharacterSlotWidget>> CharacterSlotWidgetArray;
};
