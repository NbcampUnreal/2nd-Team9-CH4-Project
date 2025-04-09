#include "SSBCharacterSelectPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetSystemLibrary.h"

ASSBCharacterSelectPlayerController::ASSBCharacterSelectPlayerController()
	: InputMappingContext(nullptr), SelectorMoveAction(nullptr), SelectAction(nullptr)
{
}

void ASSBCharacterSelectPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetShowMouseCursor(true);
	
	AddMappingContext();
	BindInputActions();
}

void ASSBCharacterSelectPlayerController::AddMappingContext() const
{
	if (const ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMappingContext)
			{
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}
}

void ASSBCharacterSelectPlayerController::BindInputActions()
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(SelectorMoveAction, ETriggerEvent::Triggered, this, &ASSBCharacterSelectPlayerController::SelectorMove);
		EnhancedInputComponent->BindAction(SelectAction, ETriggerEvent::Triggered, this, &ASSBCharacterSelectPlayerController::Select);
	}
}

void ASSBCharacterSelectPlayerController::SelectorMove(const FInputActionValue& InputValue)
{
	const FVector2D Dir = InputValue.Get<FVector2D>();
	UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("SelectorMove TEST : %.f, %.f"), Dir.X, Dir.Y));
}

void ASSBCharacterSelectPlayerController::Select(const FInputActionValue& InputValue)
{
	UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Select TEST : %s"), InputValue.Get<bool>() ? TEXT("true") : TEXT("false")));
}
