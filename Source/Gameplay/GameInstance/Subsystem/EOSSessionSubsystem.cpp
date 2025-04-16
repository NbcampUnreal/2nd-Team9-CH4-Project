#include "EOSSessionSubsystem.h"

#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSubsystemTypes.h"
#include "Kismet/GameplayStatics.h"
#include "Online/OnlineSessionNames.h"

void UEOSSessionSubsystem::CreateSession(const FName KeyName, const FString& KeyValue, const FString& MapName)
{
	OnlineSubsystem = Online::GetSubsystem(GetWorld());
	const IOnlineSessionPtr Session = OnlineSubsystem->GetSessionInterface();

	CreateLobbyDelegateHandle =
		Session->AddOnCreateSessionCompleteDelegate_Handle(FOnCreateSessionCompleteDelegate::CreateUObject(
			this,
			&ThisClass::HandleCreateLobbyCompleted));

	const TSharedRef<FOnlineSessionSettings> SessionSettings = MakeShared<FOnlineSessionSettings>();
	SessionSettings->NumPublicConnections = 4; // MaxPlayerCount
	SessionSettings->bShouldAdvertise = true; // Should be Advertised publicly
	SessionSettings->bIsDedicated = false; // Dedicated server (true) or a Listen server (false, player-hosted).
	SessionSettings->bUseLobbiesIfAvailable = true;

	SessionSettings->Settings.Add(
		KeyName, FOnlineSessionSetting((KeyValue), EOnlineDataAdvertisementType::ViaOnlineService));

	UE_LOG(LogTemp, Log, TEXT("Creating Lobby..."));

	SessionMapName = MapName;
	
	if (!Session->CreateSession(0, DefaultLobbyName, *SessionSettings))
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to create Lobby!"));
	}
}

void UEOSSessionSubsystem::HandleCreateLobbyCompleted(const FName LobbyName, const bool bWasSuccessful)
{
	OnlineSubsystem = Online::GetSubsystem(GetWorld());
	const IOnlineSessionPtr Session = OnlineSubsystem->GetSessionInterface();
	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Log, TEXT("Lobby: %s Created!"), *LobbyName.ToString());
		UE_LOG(LogTemp, Log, TEXT("SessionMapName: %s Created!"), *SessionMapName);
		// FString Map = "Game/Content/Maps/CharacterSelect?listen";
		// FURL TravelURL;
		// TravelURL.Map = Map;
		// GetWorld()->Listen(TravelURL);
		
		GetWorld()->ServerTravel(SessionMapName + "?listen");
		SessionMapName = "";

		// SetupNotifications();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to create lobby!"));
	}

	Session->ClearOnCreateSessionCompleteDelegate_Handle(CreateLobbyDelegateHandle);
	CreateLobbyDelegateHandle.Reset();
}

void UEOSSessionSubsystem::FindSessions(const FName SearchKey, const FString& SearchValue)
{
	OnlineSubsystem = Online::GetSubsystem(GetWorld());
	const IOnlineSessionPtr Session = OnlineSubsystem->GetSessionInterface();
	TSharedRef<FOnlineSessionSearch> Search = MakeShared<FOnlineSessionSearch>();

	Search->QuerySettings.SearchParams.Empty();

	Search->QuerySettings.Set(SearchKey, SearchValue, EOnlineComparisonOp::Equals);
	Search->QuerySettings.Set(SEARCH_LOBBIES, true, EOnlineComparisonOp::Equals);

	FindSessionsDelegateHandle =
		Session->AddOnFindSessionsCompleteDelegate_Handle(FOnFindSessionsCompleteDelegate::CreateUObject(
			this,
			&ThisClass::HandleFindSessionsCompleted,
			Search));

	UE_LOG(LogTemp, Log, TEXT("Finding lobby."));

	if (!Session->FindSessions(0, Search))
	{
		UE_LOG(LogTemp, Log, TEXT("Finding lobby failed."));

		Session->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsDelegateHandle);
		FindSessionsDelegateHandle.Reset();
	}
}

// ReSharper disable once CppPassValueParameterByConstReference
void UEOSSessionSubsystem::HandleFindSessionsCompleted(const bool bWasSuccessful, TSharedRef<FOnlineSessionSearch> Search)
{
	OnlineSubsystem = Online::GetSubsystem(GetWorld());
	const IOnlineSessionPtr Session = OnlineSubsystem->GetSessionInterface();
	
	if (bWasSuccessful)
	{
		if (Search->SearchResults.Num() == 0)
		{
			SessionToJoin = nullptr;
			UE_LOG(LogTemp, Log, TEXT("Search results were not found."));
			OnFindingSession.Broadcast(false);
			return;
		}

		if (Session->GetResolvedConnectString(Search->SearchResults[0], NAME_GamePort, ConnectString))
		{
			SessionToJoin = &Search->SearchResults[0];
		}
		
		UE_LOG(LogTemp, Log, TEXT("Found lobby."));
		OnFindingSession.Broadcast(true);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Find lobby failed."));
	}

	Session->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsDelegateHandle);
	FindSessionsDelegateHandle.Reset();
}

void UEOSSessionSubsystem::JoinSession()
{
	OnlineSubsystem = Online::GetSubsystem(GetWorld());
	const IOnlineSessionPtr Session = OnlineSubsystem->GetSessionInterface();

	JoinSessionDelegateHandle =
		Session->AddOnJoinSessionCompleteDelegate_Handle(FOnJoinSessionCompleteDelegate::CreateUObject(
			this,
			&ThisClass::HandleJoinSessionCompleted));

	UE_LOG(LogTemp, Log, TEXT("Joining Lobby."));

	if (!Session->JoinSession(0, DefaultLobbyName, *SessionToJoin))
	{
		UE_LOG(LogTemp, Log, TEXT("Join Lobby failed."));

		Session->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionDelegateHandle);
		JoinSessionDelegateHandle.Reset();
	}
}

void UEOSSessionSubsystem::HandleJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	OnlineSubsystem = Online::GetSubsystem(GetWorld());
	IOnlineSessionPtr Session = OnlineSubsystem->GetSessionInterface();

	if (Result == EOnJoinSessionCompleteResult::Success)
	{
		UE_LOG(LogTemp, Log, TEXT("Joined lobby."));
		
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (IsValid(PlayerController))
		{
			PlayerController->ClientTravel(ConnectString, TRAVEL_Absolute);
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Is Not Valid PlayerController"));
		}
		// SetupNotifications();
	}

	Session->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionDelegateHandle);
	JoinSessionDelegateHandle.Reset();
}

// void AEOSPlayerController::SetupNotifications()
// {
//     IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
//     IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
//
//     // In this tutorial we're only giving an example of a notification for when a participant joins/leaves the lobby. The approach is similar for other notifications. 
//     Session->AddOnSessionParticipantsChangeDelegate_Handle(FOnSessionParticipantsChangeDelegate::CreateUObject(
//         this,
//         &ThisClass::HandleParticipantChanged)); 
// }
//
// void AEOSPlayerController::HandleParticipantChanged(FName EOSLobbyName, const FUniqueNetId& NetId, bool bJoined)
// {
//     if (bJoined)
//     {
//         UE_LOG(LogTemp, Log, TEXT("A player has joined Lobby: %s"), *LobbyName.ToString()); 
//     }
//     else
//     {
//         UE_LOG(LogTemp, Log, TEXT("A player has left Lobby: %s"), *LobbyName.ToString());
//     }
// }