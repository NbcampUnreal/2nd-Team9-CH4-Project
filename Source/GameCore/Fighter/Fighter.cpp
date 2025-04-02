#include "Fighter.h"
#include "GameFramework/PlayerState.h"
#include "GameCore/Interface/PlayerStateInterface.h"
#include "MessageBus/MessageBusManager.h"
#include "InputActionValue.h"

AFighter::AFighter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AFighter::BeginPlay()
{
	Super::BeginPlay();
	
	if(UMessageBusManager* MessageBus = UMessageBusManager::GetInstance())
	{
		FMessageDelegate Delegate;
		Delegate.BindUObject(this, &AFighter::ImSleepy);
		MessageBus->Subscribe(TEXT("Test1"), Delegate);
	}
}

void AFighter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFighter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AFighter::ImSleepy(const FString& MessageType, UObject* Payload)
{
	UE_LOG(LogTemp, Display, TEXT("Test"));
}

void AFighter::IWantToSleep() const
{
	if (APlayerState* PS = GetPlayerState())
	{
		if (const IPlayerStateInterface* PlayerStateInterface = Cast<IPlayerStateInterface>(PS))
		{
			UE_LOG(LogTemp, Log, TEXT("Player Score: %d"), PlayerStateInterface->GetPlayerScore());
		}	
	}
}


void AFighter::Move(const FInputActionValue& InputValue)
{
	// Move...
	const FVector2D MovementVector = InputValue.Get<FVector2D>();

	AddMovementInput(FVector(0.0f, 1.0f, 0.0f), MovementVector.Y);
}

void AFighter::StartJump(const FInputActionValue& InputValue)
{
	// Jump...
}


