#pragma once

#include "CoreMinimal.h"
#include "NotifyWidgetInitData.h"
#include "Blueprint/UserWidget.h"

#include "NotifyWidget.generated.h"

class UButton;
class UTextBlock;

UCLASS()
class GAMEUI_API UNotifyWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual bool Initialize() override;

public:
	void SetupNotifyWidget(const FNotifyWidgetInitData& NotifyWidgetInitData);

private:
	UFUNCTION()
	void HandleConfirm();

	UFUNCTION()
	void HandleCancel();

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> NotifyTextBlock;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ConfirmButton;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ConfirmButtonTextBlock;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> CancelButton;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CancelButtonTextBlock;

	FNotifyWidgetInitData InitData;
};
