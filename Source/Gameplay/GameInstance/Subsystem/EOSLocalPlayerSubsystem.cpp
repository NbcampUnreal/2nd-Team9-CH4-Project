#include "EOSLocalPlayerSubsystem.h"

#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSubsystemTypes.h"
#include "Interfaces/OnlineIdentityInterface.h"

void UEOSLocalPlayerSubsystem::Login()
{
	OnlineSubsystem = Online::GetSubsystem(GetWorld());

	const IOnlineIdentityPtr Identity = OnlineSubsystem->GetIdentityInterface();
	const FUniqueNetIdPtr NetId = Identity->GetUniquePlayerId(0);

	if (NetId != nullptr && Identity->GetLoginStatus(0) == ELoginStatus::LoggedIn)
	{
		return;
	}

	LoginDelegateHandle = Identity->AddOnLoginCompleteDelegate_Handle(
		0,
		FOnLoginCompleteDelegate::CreateUObject(
			this,
			&ThisClass::HandleLoginCompleted));

	FString AuthType;
	FParse::Value(FCommandLine::Get(), TEXT("AUTH_TYPE="), AuthType);

	// Automatic Login Using CommandLine Arguments
	if (!AuthType.IsEmpty())
	{
		UE_LOG(LogTemp, Log, TEXT("Logging into EOS..."));

		if (!Identity->AutoLogin(0))
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to login... "));

			Identity->ClearOnLoginCompleteDelegate_Handle(0, LoginDelegateHandle);
			LoginDelegateHandle.Reset();
		}
	}
	else
	{
		FOnlineAccountCredentials Credentials("AccountPortal", "", "");

		UE_LOG(LogTemp, Log, TEXT("Logging into EOS..."));

		if (!Identity->Login(0, Credentials))
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to login... "));

			Identity->ClearOnLoginCompleteDelegate_Handle(0, LoginDelegateHandle);
			LoginDelegateHandle.Reset();
		}
	}
}

void UEOSLocalPlayerSubsystem::HandleLoginCompleted(const int32 LocalUserNum,
                                                    const bool bWasSuccessful,
                                                    const FUniqueNetId& UserId,
                                                    const FString& Error)
{
	const IOnlineIdentityPtr Identity = OnlineSubsystem->GetIdentityInterface();
	if (bWasSuccessful)
	{
		if (bWasSuccessful)
		{
			UE_LOG(LogTemp, Log, TEXT("Login callback completed!"));
			UE_LOG(LogTemp, Warning, TEXT("UserId: %s"), *UserId.ToString())
			OnLoginSuccess.Broadcast();
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("EOS login failed."));

		if (!Error.IsEmpty())
		{
			UE_LOG(LogTemp, Log, TEXT("EOS login error: %s"), *Error);
		}
		
		OnLoginFailed.Broadcast();
	}

	Identity->ClearOnLoginCompleteDelegate_Handle(LocalUserNum, LoginDelegateHandle);
	LoginDelegateHandle.Reset();
}
