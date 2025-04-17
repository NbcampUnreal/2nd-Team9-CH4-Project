#pragma once

#include "CoreMinimal.h"
#include "OnlineSessionSettings.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSubsystemSessionSettings.h"

#include "MainMenuWidget.generated.h"

class UBackgroundBlur;
class UEOSSessionSubsystem;
class UEOSLocalPlayerSubsystem;
class UNotifyWidget;

UCLASS()
class GAMEUI_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual bool Initialize() override;
	virtual void BeginDestroy() override;
	
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	TObjectPtr<UUserWidget> MainMenuButtons;
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	TObjectPtr<UBackgroundBlur> LoadingWidget;

private:
	UFUNCTION()
	void OnLoginSuccess();
	UFUNCTION()
	void OnLoginFailed();
	UFUNCTION()
	void RetryLogin() const;

	UFUNCTION()
	void OnFindSession(bool bIsFinding);
	UFUNCTION()
	void JoinSession() const;

	void SetupLoginFailedNotify();
	void SetupFindSessionNotify(bool bIsFinding);
	void AddOrRemoveLoginResultDelegate(bool bIsAdd);
	void AddOrRemoveFindSessionResultDelegate(bool bIsAdd);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UNotifyWidget> NotifyWidgetClass;

	UPROPERTY()
	TObjectPtr<UNotifyWidget> NotifyWidget;

	TObjectPtr<UEOSLocalPlayerSubsystem> LocalPlayerSubsystem;
	TObjectPtr<UEOSSessionSubsystem> SessionSubsystem;

	FOnlineSessionSearchResult SessionToJoin;
};
