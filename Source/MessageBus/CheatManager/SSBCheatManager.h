#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "SSBCheatManager.generated.h"

UCLASS()
class MESSAGEBUS_API USSBCheatManager : public UCheatManager
{
	GENERATED_BODY()

public:
	UFUNCTION(exec)
	void CreateRoom(const FString& RoomName);
	
	UFUNCTION(exec)
	void JoinRoom(const FString& RoomName);
	
	UFUNCTION(exec)
	void StartGame();

	UFUNCTION(exec)
	void RoomPlayerList() const;

protected:
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnFindSessionsComplete(bool bWasSuccessful);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	// JoinRoom variables
	FString PendingRoomNameForJoin;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;
};
