#include "SSBCheatManager.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"

void USSBCheatManager::CreateRoom(const FString& RoomName)
{
    const IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
    if (!OnlineSub)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No OnlineSubsystem found"));
        return;
    }

    const IOnlineSessionPtr SessionInterface = OnlineSub->GetSessionInterface();
    if (!SessionInterface.IsValid())
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No SessionInterface found"));
        return;
    }
    
    SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &USSBCheatManager::OnCreateSessionComplete);

    TSharedPtr<FOnlineSessionSettings> SessionSettings = MakeShareable(new FOnlineSessionSettings());
    SessionSettings->bIsLANMatch = true;
    SessionSettings->NumPublicConnections = 4;
    SessionSettings->bShouldAdvertise = true;
    SessionSettings->bUsesPresence = true;
    
    SessionSettings->Set(FName("ROOM_NAME"), RoomName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

    // 로컬 플레이어의 고유 아이디 획득
    // ReSharper disable once CppTooWideScopeInitStatement
    const ULocalPlayer* LocalPlayer = GetOuterAPlayerController()->GetLocalPlayer();
    if (LocalPlayer && LocalPlayer->GetPreferredUniqueNetId().IsValid())
    {
        SessionInterface->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, *SessionSettings);
        
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Creating room: %s"), *RoomName));
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Invalid local player!"));
    }
}

void USSBCheatManager::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
    const IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
    if (!OnlineSub)
        return;

    if (const IOnlineSessionPtr SessionInterface = OnlineSub->GetSessionInterface(); SessionInterface.IsValid())
    {
        SessionInterface->ClearOnCreateSessionCompleteDelegates(this);
    }
    
    if (bWasSuccessful)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Room created: %s"), *SessionName.ToString()));

        /* 고민 애초에 호스트가 리슨서버로 Room Level에 있는게 아닌거같음
         * 방을 생성할때 전환을 하는게 맞아보임..
         * 메인메뉴 -> 방 -> 인게임
         * 이라 치면 이게 맞아보임
         * 아니였음 망할
        */
        //UGameplayStatics::OpenLevel(GetWorld(), TEXT("TestRoom"), true, TEXT("listen"));
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Failed to create room: %s"), *SessionName.ToString()));
    }
}

void USSBCheatManager::JoinRoom(const FString& RoomName)
{
    const IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
    if (!OnlineSub)
    {
        UE_LOG(LogTemp, Warning, TEXT("No OnlineSubsystem found!"));
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No OnlineSubsystem found!"));
        return;
    }

    IOnlineSessionPtr SessionInterface = OnlineSub->GetSessionInterface();
    if (!SessionInterface.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("No SessionInterface found!"));
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No SessionInterface found!"));
        return;
    }

    // Join시 검색할 방 이름
    PendingRoomNameForJoin = RoomName;

    // 검색 객체
    SessionSearch = MakeShareable(new FOnlineSessionSearch());
    SessionSearch->bIsLanQuery = true;
    SessionSearch->MaxSearchResults = 10;
    SessionSearch->QuerySettings.Set(FName(TEXT("SEARCH_PRESENCE")), true, EOnlineComparisonOp::Equals);

    // FindSession 델리게이트
    SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &USSBCheatManager::OnFindSessionsComplete);

    // 로컬 플레이어 고유 아이디 획득 후 세션 검색 시작
    const ULocalPlayer* LocalPlayer = GetOuterAPlayerController()->GetLocalPlayer();
    if (LocalPlayer && LocalPlayer->GetPreferredUniqueNetId().IsValid())
    {
        SessionInterface->FindSessions(*LocalPlayer->GetPreferredUniqueNetId(), SessionSearch.ToSharedRef());
        UE_LOG(LogTemp, Warning, TEXT("Searching for room: %s"), *RoomName);
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Searching for room: %s"), *RoomName));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid local player!"));
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Invalid local player!"));
    }
}

void USSBCheatManager::OnFindSessionsComplete(bool bWasSuccessful)
{
    IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
    if (!OnlineSub)
        return;

    IOnlineSessionPtr SessionInterface = OnlineSub->GetSessionInterface();
    if (SessionInterface.IsValid())
    {
        SessionInterface->ClearOnFindSessionsCompleteDelegates(this);
    }

    if (!bWasSuccessful || !SessionSearch.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("Session search failed."));
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Session search failed."));
        return;
    }

    // 검색 결과에서 커스텀 ROOM_NAME 값과 일치하는 세션 찾기
    for (const FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
    {
        FString FoundRoomName;
        if (SearchResult.Session.SessionSettings.Get(FName("ROOM_NAME"), FoundRoomName))
        {
            if (FoundRoomName.Equals(PendingRoomNameForJoin, ESearchCase::IgnoreCase))
            {
                UE_LOG(LogTemp, Warning, TEXT("Found matching room: %s"), *FoundRoomName);
                GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Found room: %s, joining..."), *FoundRoomName));

                // JoinSession 델리게이트
                SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &USSBCheatManager::OnJoinSessionComplete);
                
                const ULocalPlayer* LocalPlayer = GetOuterAPlayerController()->GetLocalPlayer();
                if (LocalPlayer && LocalPlayer->GetPreferredUniqueNetId().IsValid())
                {
                    SessionInterface->JoinSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, SearchResult);
                }
                return;
            }
        }
    }
    
    UE_LOG(LogTemp, Warning, TEXT("No room found with name: %s"), *PendingRoomNameForJoin);
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("No room found with name: %s"), *PendingRoomNameForJoin));
}

void USSBCheatManager::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
    IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
    if (!OnlineSub)
        return;

    IOnlineSessionPtr SessionInterface = OnlineSub->GetSessionInterface();
    if (SessionInterface.IsValid())
    {
        SessionInterface->ClearOnJoinSessionCompleteDelegates(this);
    }

    if (Result == EOnJoinSessionCompleteResult::Success)
    {
        FString ConnectString;
        const ULocalPlayer* LocalPlayer = GetOuterAPlayerController()->GetLocalPlayer();
        if (SessionInterface->GetResolvedConnectString(SessionName, ConnectString) && GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Joining room at: %s"), *ConnectString));
            
            if (APlayerController* PC = GetOuterAPlayerController())
            {
                PC->ClientTravel(ConnectString, TRAVEL_Absolute);
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to join session %s, result: %d"), *SessionName.ToString(), (int32)Result);
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Failed to join room: %s"), *SessionName.ToString()));
    }
}

void USSBCheatManager::StartGame()
{
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Starting game..."));
    UE_LOG(LogTemp, Warning, TEXT("Starting game..."));

    if (UWorld* World = GetWorld())
    {
        World->ServerTravel(TEXT("/Game/Maps/TestMap?listen"));
    }
}

void USSBCheatManager::RoomPlayerList() const
{
    const UWorld* World = GetWorld();
    if (!World)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No world found!"));
        return;
    }
    
    AGameStateBase* GameState = World->GetGameState();
    if (!GameState)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No GameState found!"));
        return;
    }
    
    FString PlayerList = TEXT("Players in current session:\n");
    for (const APlayerState* PlayerState : GameState->PlayerArray)
    {
        if (PlayerState)
        {
            PlayerList.Append(FString::Printf(TEXT(" - %s\n"), *PlayerState->GetPlayerName()));
        }
    }
    GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, PlayerList);
    UE_LOG(LogTemp, Warning, TEXT("%s"), *PlayerList);
}
