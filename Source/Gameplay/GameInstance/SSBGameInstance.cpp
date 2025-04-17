#include "SSBGameInstance.h"

#include "MessageBus/MessageBusManager.h"
#include "Subsystem/EOSSessionSubsystem.h"
#include "Subsystem/EOSLocalPlayerSubsystem.h"

USSBGameInstance::USSBGameInstance()
{
	CurrentLevelTag = FGameplayTag::RequestGameplayTag(FName("GameLevel.MainMenu"));

	PlayerIndex = 0;
	SelectedCharacterTypeIndex = 0;
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

void USSBGameInstance::StartGameInstance()
{
	Super::StartGameInstance();

	EOSSessionSubsystem = GetSubsystem<UEOSSessionSubsystem>();
}

void USSBGameInstance::ChangeLevel(const FName& LevelName)
{
	CurrentLevelTag = FGameplayTag::RequestGameplayTag(LevelName);
}

void USSBGameInstance::CreateEOSSession(const FName KeyName, const FString& KeyValue, const FString& MapName)
{
	if (IsValid(EOSSessionSubsystem))
	{
		EOSSessionSubsystem->CreateSession(KeyName, KeyValue, MapName);
	}
}

void USSBGameInstance::FindEOSSessions(const FName SearchKey, const FString& SearchValue)
{
	if (IsValid(EOSSessionSubsystem))
	{
		EOSSessionSubsystem->FindSessions(SearchKey, SearchValue);
	}
}

void USSBGameInstance::JoinEOSSession()
{
	if (IsValid(EOSSessionSubsystem))
	{
		EOSSessionSubsystem->JoinSession();
	}
}
