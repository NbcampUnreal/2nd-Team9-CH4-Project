#include "PlayerInputComponent.h"
#include "../SSBPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameCore/Fighter/Fighter.h"
#include "InputActionValue.h"

UPlayerInputComponent::UPlayerInputComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
}

void UPlayerInputComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (const ASSBPlayerController* PC = Cast<ASSBPlayerController>(GetOwner()))
	{
		if (PC->IsLocalController())
		{
			AddMappingContext(PC);
			BindActions(PC);	
		}
	}
}


void UPlayerInputComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

void UPlayerInputComponent::AddMappingContext(const ASSBPlayerController* PlayerController) const
{
	if (const ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
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

void UPlayerInputComponent::BindActions(const ASSBPlayerController* PlayerController)
{
	if (AFighter* Fighter = Cast<AFighter>(PlayerController->GetPawn()))
	{
		if (UEnhancedInputComponent* InputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			InputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, Fighter, &AFighter::Move);
			InputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, Fighter, &AFighter::StartJump);
		}
	}
}

