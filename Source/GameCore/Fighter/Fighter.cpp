#include "Fighter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameCore/Interface/PlayerStateInterface.h"
#include "GameFramework/SpringArmComponent.h"
#include "MessageBus/MessageBusManager.h"
#include "GameFramework/PlayerState.h"
#include "Camera/CameraComponent.h"
#include "InputActionValue.h"
#include "GameCore/Ability/AbilityManager/AbilityManager.h"


FGameplayTag AFighter::AttackTag = FGameplayTag::RequestGameplayTag(TEXT("PlayerState.Attack"));
FGameplayTag AFighter::BaseTag = FGameplayTag::RequestGameplayTag(TEXT("PlayerState.Base"));
FGameplayTag AFighter::JumpTag = FGameplayTag::RequestGameplayTag(TEXT("PlayerState.Base.Jump"));

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

	//Test
	GetGameInstance()->GetSubsystem<UAbilityManager>()->Initialize();
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

	CurrentPlayerTag = FGameplayTag::RequestGameplayTag(FName("PlayerState.Base.Stand.Idle"));
	CurrentStandTag = "Stand";
	
	//Test
	GetGameInstance()->GetSubsystem<UAbilityManager>()->UpdateCharacter(this);
}

void AFighter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	if (!AbilityTagContainer.HasTag(AttackTag) && CurrentPlayerTag != JumpTag)
	{
		CurrentPlayerTag = FGameplayTag::RequestGameplayTag(FName(*FString::Printf(TEXT("PlayerState.Base.%s.Idle"), *CurrentStandTag)));
	}
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
	/* 공격중에 실제 이동을 처리할건지는 나중에 판단 */
	if (AbilityTagContainer.HasTag(AttackTag))
	{
		return;
	}
	
	const FVector2D MovementVector = InputValue.Get<FVector2D>();
	AddMovementInput(FVector(1.0f, 0.0f, 0.0f), MovementVector.X);

	/* 점프중에는 이동은 하지만 애니메이션은 처리 X */
	if (CurrentPlayerTag == JumpTag)
	{
		return;
	}
	
	CurrentStandTag = "Stand";
	if (MovementVector.Y < 0.0f)
	{
		CurrentStandTag = "Crouch";
	}

	FString MoveTag = "Idle";
	if (MovementVector.X > 0.0f)
	{
		MoveTag = "WalkForward";
	}
	else if (MovementVector.X < 0.0f)
	{
		MoveTag = "WalkBackward";
	}
	
	CurrentPlayerTag = FGameplayTag::RequestGameplayTag(FName(*FString::Printf(TEXT("PlayerState.Base.%s.%s"), *CurrentStandTag, *MoveTag)));
}

void AFighter::StartJump(const FInputActionValue& InputValue)
{
	CurrentPlayerTag = FGameplayTag::RequestGameplayTag(FName("PlayerState.Base.Jump"));
	Jump();
}

void AFighter::SetChangeStandTag()
{
	if (CurrentStandTag == "Stand")
	{
		CurrentStandTag = "Crouch";
	}
	else
	{
		CurrentStandTag = "Stand";
	}
}

FGameplayTagContainer& AFighter::GetCurrentTags()
{
	 return AbilityTagContainer; 
}
