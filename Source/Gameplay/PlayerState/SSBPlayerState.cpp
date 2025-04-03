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
		MessageBus->BroadcastMessage(TEXT("Test"), this);
	}
}
