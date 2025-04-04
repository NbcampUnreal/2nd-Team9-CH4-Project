#include "Fighter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameCore/Interface/PlayerStateInterface.h"
#include "GameFramework/SpringArmComponent.h"
#include "MessageBus/MessageBusManager.h"
#include "GameFramework/PlayerState.h"
#include "Camera/CameraComponent.h"
#include "InputActionValue.h"

AFighter::AFighter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetWorldRotation(FRotator(-5.f, -90.f, 0.f));
	SpringArm->SetAbsolute(false, false, false);
	SpringArm->TargetArmLength = 1000.0f;
	SpringArm->bDoCollisionTest = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	Camera->SetFieldOfView(40.0f);
	
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
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
	/* Allowed only when the State is not Attack
	 * if(PlayerState == EPlayerState::Attack)
	 * {
	 * 		return;
	 * };
	*/
	
	const FVector2D MovementVector = InputValue.Get<FVector2D>();

	AddMovementInput(FVector(1.0f, 0.0f, 0.0f), MovementVector.X);
}

void AFighter::StartJump(const FInputActionValue& InputValue)
{
	
	Jump();
}

