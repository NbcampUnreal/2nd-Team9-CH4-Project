#include "SSBGameInstance.h"

#include "MessageBus/MessageBusManager.h"

USSBGameInstance::USSBGameInstance()
{
	CurrentLevelTag = FGameplayTag::RequestGameplayTag(FName("GameLevel.MainMenu"));
}

void USSBGameInstance::Init()
{
	Super::Init();
	
	// Initialize the message bus manager
	if (UMessageBusManager* MessageBus = UMessageBusManager::GetInstance())
	{
		//FMessageDelegate Delegate;
		//Delegate.BindUObject(this, &USSBGameInstance::OnMessageReceived);
		//MessageBus->Subscribe(TEXT("Test1"), Delegate);
	}
}

void USSBGameInstance::Shutdown()
{
	Super::Shutdown();
}

void USSBGameInstance::ChangeLevel(const FName& LevelName)
{
	CurrentLevelTag = FGameplayTag::RequestGameplayTag(LevelName);
}
