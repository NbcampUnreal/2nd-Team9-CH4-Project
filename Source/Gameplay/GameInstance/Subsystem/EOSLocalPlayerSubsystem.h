#pragma once

#include "CoreMinimal.h"
#include "OnlineSubsystem.h"
#include "Subsystems/LocalPlayerSubsystem.h"

#include "EOSLocalPlayerSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLoginSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLoginFailed);

UCLASS()
class GAMEPLAY_API UEOSLocalPlayerSubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()

public:
	void Login();
	void HandleLoginCompleted(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error);
	FDelegateHandle LoginDelegateHandle;

	FOnLoginSuccess OnLoginSuccess;
	FOnLoginFailed OnLoginFailed;
private:
	IOnlineSubsystem* OnlineSubsystem;
};
