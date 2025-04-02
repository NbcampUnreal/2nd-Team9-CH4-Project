#include "SSBPlayerState.h"
#include "MessageBus/MessageBusManager.h"

ASSBPlayerState::ASSBPlayerState()
{
	
}

void ASSBPlayerState::BeginPlay()
{
	Super::BeginPlay();
	
	if(UMessageBusManager* MessageBus = UMessageBusManager::GetInstance())
	{
		MessageBus->BroadcastMessage(TEXT("Test1"), this);
		MessageBus->BroadcastMessage(TEXT("Test2"), this);
		MessageBus->BroadcastMessage(TEXT("Test3"), this);
	}
}
