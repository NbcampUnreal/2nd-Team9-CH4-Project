#include "CharacterSelectPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetSystemLibrary.h"

ACharacterSelectPlayerController::ACharacterSelectPlayerController()
	: InputMappingContext(nullptr), SelectorMoveAction(nullptr), SelectAction(nullptr)
{
}

void ACharacterSelectPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetShowMouseCursor(true);
	
	// AddMappingContext();
	// BindInputActions();
}

void ACharacterSelectPlayerController::AddMappingContext() const
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

void ACharacterSelectPlayerController::BindInputActions()
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(SelectorMoveAction, ETriggerEvent::Triggered, this, &ACharacterSelectPlayerController::SelectorMove);
		EnhancedInputComponent->BindAction(SelectAction, ETriggerEvent::Triggered, this, &ACharacterSelectPlayerController::Select);
	}
}

void ACharacterSelectPlayerController::SelectorMove(const FInputActionValue& InputValue)
{
	const FVector2D Dir = InputValue.Get<FVector2D>();
	UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("SelectorMove TEST : %.f, %.f"), Dir.X, Dir.Y));
}

void ACharacterSelectPlayerController::Select(const FInputActionValue& InputValue)
{
	UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Select TEST : %s"), InputValue.Get<bool>() ? TEXT("true") : TEXT("false")));
}
