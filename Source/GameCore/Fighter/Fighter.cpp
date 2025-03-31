#include "Fighter.h"
#include "InputActionValue.h"

AFighter::AFighter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AFighter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFighter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFighter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AFighter::Move(const FInputActionValue& InputValue)
{
	const FVector2D MovementVector = InputValue.Get<FVector2D>();

	AddMovementInput(FVector(0.0f, 1.0f, 0.0f), MovementVector.Y);
}

void AFighter::StartJump(const FInputActionValue& InputValue)
{
	
}


