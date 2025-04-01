#include "SSBPlayerState.h"
#include "MessageBus/MessageBus.h"

ASSBPlayerState::ASSBPlayerState()
{
	
}

void ASSBPlayerState::BeginPlay()
{
	Super::BeginPlay();
	
	if(UMessageBus* MessageBus = UMessageBus::GetInstance())
	{
		MessageBus->BroadcastMessage(TEXT("Test"), this);
	}
}
