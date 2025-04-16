#include "MainMenuWidget.h"

#include "Components/BackgroundBlur.h"
#include "Gameplay/GameInstance/Subsystem/EOSLocalPlayerSubsystem.h"
#include "Gameplay/GameInstance/Subsystem/EOSSessionSubsystem.h"
#include "Gameplay/PlayerController/MainMenuPlayerController.h"
#include "GameUI/UI/Notify//NotifyWidget.h"
#include "Kismet/KismetSystemLibrary.h"

bool UMainMenuWidget::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	const AMainMenuPlayerController* PlayerController = Cast<AMainMenuPlayerController>(GetOwningPlayer());
	if (IsValid(PlayerController))
	{
		LocalPlayerSubsystem = PlayerController->GetEOSLocalPlayerSubsystem();
		AddOrRemoveLoginResultDelegate(true);
	}

	const UGameInstance* GameInstance = GetGameInstance();
	if (IsValid(GameInstance))
	{
		SessionSubsystem = GameInstance->GetSubsystem<UEOSSessionSubsystem>();
		AddOrRemoveFindSessionResultDelegate(true);
	}

	if (IsValid(NotifyWidgetClass))
	{
		NotifyWidget = CreateWidget<UNotifyWidget>(GetWorld(), NotifyWidgetClass);
		SetupLoginFailedNotify();
	}

	return true;
}

void UMainMenuWidget::BeginDestroy()
{
	Super::BeginDestroy();

	AddOrRemoveLoginResultDelegate(false);
	AddOrRemoveFindSessionResultDelegate(false);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UMainMenuWidget::OnLoginSuccess()
{
	if (IsValid(MainMenuButtons))
	{
		MainMenuButtons->SetVisibility(ESlateVisibility::Visible);
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UMainMenuWidget::OnLoginFailed()
{
	if (IsValid(NotifyWidget))
	{
		SetupLoginFailedNotify();
		NotifyWidget->AddToViewport();
	}
}

void UMainMenuWidget::RetryLogin() const
{
	if (IsValid(LocalPlayerSubsystem))
	{
		LocalPlayerSubsystem->Login();
		NotifyWidget->RemoveFromParent();
	}
}

void UMainMenuWidget::OnFindSession(const bool bIsFinding)
{
	if (IsValid(NotifyWidget))
	{
		SetupFindSessionNotify(bIsFinding);
		NotifyWidget->AddToViewport();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("NotifyWidget Is Not Valid"));
	}
}

void UMainMenuWidget::JoinSession() const
{
	if (IsValid(SessionSubsystem))
	{
		SessionSubsystem->JoinSession();
		NotifyWidget->RemoveFromParent();
	}
}

void UMainMenuWidget::SetupLoginFailedNotify()
{
	if (IsValid(NotifyWidget))
	{
		FNotifyWidgetInitData InitData;
		InitData.NotifyWidgetText = FNotifyWidgetText(
			FText::FromString("Required Login"),
			FText::FromString("Login"),
			FText::FromString("QuitGame")
		);
		InitData.OnConfirmButtonClickedDelegate.BindUFunction(this, FName("RetryLogin"));
		InitData.OnCancelButtonClickedDelegate.BindLambda(
			[&]()
			{
				UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, false);
				NotifyWidget->RemoveFromParent();
			});

		NotifyWidget->SetupNotifyWidget(InitData);
	}
}

void UMainMenuWidget::SetupFindSessionNotify(const bool bIsFinding)
{
	if (IsValid(NotifyWidget))
	{
		FNotifyWidgetInitData InitData;

		const FString NotifyText = bIsFinding ? TEXT("Finding.") : TEXT("Not Finding.");
		const FString ConfirmButtonText = bIsFinding ? TEXT("Join") : TEXT("");

		InitData.NotifyWidgetText = FNotifyWidgetText(
			FText::FromString(FString::Printf(TEXT("Session %s"), *NotifyText)),
			FText::FromString(ConfirmButtonText),
			FText::FromString("Cancel")
		);
		if (bIsFinding)
		{
			InitData.OnConfirmButtonClickedDelegate.BindUFunction(this, FName("JoinSession"));
		}

		InitData.OnCancelButtonClickedDelegate.BindLambda(
			[NotifyWidget = NotifyWidget, LoadingWidget = LoadingWidget]()
			{
				LoadingWidget->SetVisibility(ESlateVisibility::Hidden);
				NotifyWidget->RemoveFromParent();
			});

		NotifyWidget->SetupNotifyWidget(InitData);
	}
}

void UMainMenuWidget::AddOrRemoveLoginResultDelegate(const bool bIsAdd)
{
	if (IsValid(LocalPlayerSubsystem))
	{
		if (bIsAdd)
		{
			LocalPlayerSubsystem->OnLoginSuccess.AddUniqueDynamic(this, &ThisClass::OnLoginSuccess);
			LocalPlayerSubsystem->OnLoginFailed.AddUniqueDynamic(this, &ThisClass::OnLoginFailed);
		}
		else
		{
			LocalPlayerSubsystem->OnLoginSuccess.RemoveDynamic(this, &ThisClass::OnLoginSuccess);
			LocalPlayerSubsystem->OnLoginFailed.RemoveDynamic(this, &ThisClass::OnLoginFailed);
		}
	}
}

void UMainMenuWidget::AddOrRemoveFindSessionResultDelegate(const bool bIsAdd)
{
	if (IsValid(SessionSubsystem))
	{
		if (bIsAdd)
		{
			SessionSubsystem->OnFindingSession.AddUniqueDynamic(this, &ThisClass::OnFindSession);
		}
		else
		{
			SessionSubsystem->OnFindingSession.RemoveDynamic(this, &ThisClass::OnFindSession);
		}
	}
}
