#pragma once

#include "CoreMinimal.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "EOSSessionSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFindingSession, bool, bIsFinding);

UCLASS()
class GAMEPLAY_API UEOSSessionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

public:
	
	void CreateSession(FName KeyName, const FString& KeyValue, const FString& MapName);
	void HandleCreateLobbyCompleted(FName LobbyName, bool bWasSuccessful);
	FDelegateHandle CreateLobbyDelegateHandle;

	void StartSession();
	void HandleStartSessionCompleted(FName LobbyName, bool bWasSuccessful); 
	FDelegateHandle StartSessionDelegateHandle;
	
	
	void FindSessions(FName SearchKey, const FString& SearchValue);
	void HandleFindSessionsCompleted(bool bWasSuccessful, TSharedRef<FOnlineSessionSearch> Search);
	FDelegateHandle FindSessionsDelegateHandle;
	
	void JoinSession();
	void HandleJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	FDelegateHandle JoinSessionDelegateHandle;

	FOnFindingSession OnFindingSession;
	
private:
	IOnlineSubsystem* OnlineSubsystem;
	IOnlineSessionPtr Session;

	FName DefaultLobbyName = "LobbyName";
	FName DefaultSearchKey = "KeyName";
	FString DefaultSearchValue = "KeyValue";

	FString ConnectString;

	FString SessionMapName;

	FOnlineSessionSearchResult* SessionToJoin;
	
	// // Function used to setup our listeners to lobby notification events - example on participant change only.
	// void SetupNotifications();
	//
	// // Callback function. This function will run when a lobby participant joins / leaves.
	// void HandleParticipantChanged(FName EOSLobbyName, const FUniqueNetId& NetId, bool bJoined); 
};
