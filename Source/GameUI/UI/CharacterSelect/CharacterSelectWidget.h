#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterSelectWidget.generated.h"

UCLASS(Abstract, Blueprintable)
class GAMEUI_API UCharacterSelectWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitWidget();
};
