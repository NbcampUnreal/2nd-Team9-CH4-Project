#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterSlotWidget.generated.h"

class ACharacterSelectPawn;
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
	void InitWidget(bool bIsMyWidget, int32 Index);

private:
	
	bool CanButtonClickAction() const;

	UFUNCTION(BlueprintCallable)
	void HandleButtonClicked(bool bIsUpButton);

	void ChangeCharacterModel();
	UFUNCTION()
	void ChangeIconTexture(UTexture2D* IconTexture);

	ACharacterSelectPawn* GetCharacterSelectPawn(const int32 Index) const;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> OutLineBorder;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> SelectUpButton;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> SelectDownButton;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> CharacterIconImage;

	UPROPERTY(EditDefaultsOnly)
	TArray<FGameplayTag> CharacterTypeTagArray;
	
	int32 CharacterTypeTagIndex = 0;

	TObjectPtr<ACharacterSelectPlayerController> OwnerController;
};
