#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterSlotWidget.generated.h"

class UImage;
class ACharacterSelectPlayerController;
struct FGameplayTag;
class UBorder;
class UButton;

UCLASS(Abstract, Blueprintable)
class GAMEUI_API UCharacterSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitWidget(bool bIsMyWidget) const;

private:
	
	bool CanButtonClickAction() const;

	UFUNCTION(BlueprintCallable)
	void HandleButtonClicked(bool bIsUpButton);

	void ChangeCharacterModel();
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> OutLineBorder;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> SelectUpButton;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> SelectDownButton;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> CharacterIconImage;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> CharacterPreviewImage;

	UPROPERTY(EditDefaultsOnly)
	TArray<FGameplayTag> CharacterTypeTagArray;
	
	int32 CharacterTypeTagIndex = 0;

	TObjectPtr<ACharacterSelectPlayerController> OwnerController;
};
