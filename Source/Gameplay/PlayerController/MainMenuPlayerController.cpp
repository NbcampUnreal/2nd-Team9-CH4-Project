#include "MainMenuPlayerController.h"

#include "Gameplay/GameInstance/Subsystem/EOSLocalPlayerSubsystem.h"

void AMainMenuPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UEOSLocalPlayerSubsystem* EOSLocalPlayerSubsystem = GetLocalPlayer()->GetSubsystem<UEOSLocalPlayerSubsystem>();
	if (IsValid(EOSLocalPlayerSubsystem))
	{
		EOSLocalPlayerSubsystem->Login();
	}
}

UEOSLocalPlayerSubsystem* AMainMenuPlayerController::GetEOSLocalPlayerSubsystem() const
{
	const ULocalPlayer* LocalPlayer = GetLocalPlayer();
	if (IsValid(LocalPlayer))
	{
		return LocalPlayer->GetSubsystem<UEOSLocalPlayerSubsystem>();
	}
	
	return nullptr;
}
