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

USTRUCT()
struct FCharacterSlotData
{
	GENERATED_BODY()

	FCharacterSlotData(): bIsMyWidget(false),
						  PlayerIndex(0),
						  MaxCharacterTypeIndex(0),
						  OwnerPlayerController(nullptr),
						  IconTexture(nullptr)
	{
	}

	FCharacterSlotData(const bool bNewIsMyWidget,
					   const int32 NewPlayerIndex,
					   const int32 NewMaxCharacterTypeIndex,
					   APlayerController* NewOwnerPlayerController,
					   UTexture2D* NewIconTexture):
		bIsMyWidget(bNewIsMyWidget),
		PlayerIndex(NewPlayerIndex),
		MaxCharacterTypeIndex(NewMaxCharacterTypeIndex),
		OwnerPlayerController(NewOwnerPlayerController), IconTexture(NewIconTexture)
	{
	}

	bool bIsMyWidget;
	int32 PlayerIndex;
	int32 MaxCharacterTypeIndex;
	UPROPERTY()
	APlayerController* OwnerPlayerController;
	UPROPERTY()
	UTexture2D* IconTexture;
};

UCLASS(Abstract, Blueprintable)
class GAMEUI_API UCharacterSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void UpdateWidget(const FCharacterSlotData& CharacterSlotData);

	void UpdateIconTexture(UTexture2D* IconTexture) const;
	void UpdatePlayerReady(bool bInIsReady);

	FORCEINLINE bool IsHostWidget() const { return PlayerIndex == 0; }

private:
	bool CanButtonClickAction() const;
	UFUNCTION(BlueprintCallable)
	void HandleSelectButtonClicked(bool bIsNextButton);

	void ChangeCharacterModel() const;

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

	UPROPERTY(VisibleDefaultsOnly)
	FColor ReadyColor = FColor::White;

	int32 PlayerIndex = 0;

	int32 CharacterTypeIndex = 0;
	int32 MaxCharacterTypeIndex = 4;

	TObjectPtr<ACharacterSelectPlayerController> OwnerController;

	bool bIsReady = false;
};
