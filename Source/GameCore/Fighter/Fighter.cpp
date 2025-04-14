#pragma optimize("",off)

#include "Fighter.h"
#include "GameCore/Ability/AbilityManager/AbilityManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameCore/Interface/PlayerStateInterface.h"
#include "GameCore/Components/Hit//HitComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "MessageBus/MessageBusManager.h"
#include "GameFramework/PlayerState.h"
#include "Camera/CameraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameCore/Camera/SSBCamera.h"
#include "InputActionValue.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "AI/NavigationSystemBase.h"

FGameplayTag AFighter::AttackTag = FGameplayTag::RequestGameplayTag(TEXT("PlayerState.Attack"));
FGameplayTag AFighter::BaseTag = FGameplayTag::RequestGameplayTag(TEXT("PlayerState.Base"));
FGameplayTag AFighter::JumpTag = FGameplayTag::RequestGameplayTag(TEXT("PlayerState.Base.Jump"));
FGameplayTag AFighter::IdleTag = FGameplayTag::RequestGameplayTag(TEXT("PlayerState.Base.Stand.Idle"));
FGameplayTag AFighter::LandTag = FGameplayTag::RequestGameplayTag(TEXT("PlayerState.Base.Land"));
FGameplayTag AFighter::HitTag = FGameplayTag::RequestGameplayTag(TEXT("PlayerState.Base.Hit"));
FGameplayTag AFighter::CrouchTag = FGameplayTag::RequestGameplayTag(TEXT("PlayerState.Base.Crouch.Idle"));

AFighter::AFighter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetWorldRotation(FRotator(-5.f, -90.f, 0.f));
	SpringArm->SetAbsolute(false, true, false);
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

	CurrentPlayerTag = FGameplayTag::RequestGameplayTag(FName("PlayerState.Base.Stand.Idle"));
	CurrentStandTag = "Stand";

	if (NiagaraEffect)
	{
		// 추후 TArray에 담긴 컴포넌트로 1P, 2P 나눠서 색깔 지정한다던가 그래야 할듯
		NiagaraComponents.Add(UNiagaraFunctionLibrary::SpawnSystemAttached(
			NiagaraEffect,                           // Niagara 이펙트 에셋
			GetMesh(),                               // 부착할 컴포넌트 (스켈레탈 메쉬)
			FName(TEXT("Hand_R")),                    // 부착할 본의 이름 (예: 오른손)
			FVector::ZeroVector,                      // 위치 오프셋
			FRotator::ZeroRotator,                    // 회전 오프셋
			EAttachLocation::SnapToTarget,            // 부착 위치 룰
			false                                      // 이펙트 자동 제거 (재생 완료 후)
		));

		NiagaraComponents.Add(UNiagaraFunctionLibrary::SpawnSystemAttached(
		NiagaraEffect,                           // Niagara 이펙트 에셋
		GetMesh(),                               // 부착할 컴포넌트 (스켈레탈 메쉬)
		FName(TEXT("Hand_L")),                    // 부착할 본의 이름 (예: 오른손)
		FVector::ZeroVector,                      // 위치 오프셋
		FRotator::ZeroRotator,                    // 회전 오프셋
		EAttachLocation::SnapToTarget,            // 부착 위치 룰
		false                                      // 이펙트 자동 제거 (재생 완료 후)
		));
	}
	GetGameInstance()->GetSubsystem<UAbilityManager>()->UpdateCharacter(this);

	


}

void AFighter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/* 체크해야됨 상대를 바라보고 있는 쪽으로 180도 회전 해야됨
	if(GetLocation().X < EnemyInstance->GetActorLocation().X)
	{
		적이 나보다 오른쪽에 있는경우
		bIsLookingRight = true;
	}
	else
	{
		bIsLookingRight = false;
		SetActorRotation(FRotator(0.0f, 180.0f, 0.0f));
	}
	 */
	// Test
	/*if(GetActorLocation().X < 0)
	{
		bLookingRight = true;
	}
	else
	{
		bLookingRight = false;
		SetActorRotation(FRotator(0.0f, 180.0f, 0.0f));
	}*/
	if (!AbilityTagContainer.HasTag(AttackTag) && !bCheckTickCrouch && CurrentPlayerTag.MatchesTag(CrouchTag))
	{
		bCheckTickCrouch = false;
		CurrentPlayerTag = IdleTag;
		CurrentStandTag = "Stand";
	}

	if (!bLookingRight)
	{
		SetActorRotation(FRotator(0.0f, 180.0f, 0.0f));
	}
	
	if (!AbilityTagContainer.HasTag(AttackTag) && CurrentPlayerTag != JumpTag && CurrentPlayerTag != LandTag &&
		!CurrentPlayerTag.MatchesTag(HitTag))
	{
		CurrentPlayerTag = FGameplayTag::RequestGameplayTag(FName(*FString::Printf(TEXT("PlayerState.Base.%s.Idle"), *CurrentStandTag)));
		//CurrentPlayerTag = FGameplayTag::RequestGameplayTag(FName(*FString::Printf(TEXT("PlayerState.Base.Stand.Idle"))));
	}

	if (CurrentLockTag.MatchesTag(BaseTag))
	{
		CurrentPlayerTag = CurrentLockTag;
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

void AFighter::Move(const FInputActionValue& InputValue)
{
	/* 공격중에 실제 이동을 처리할건지는 나중에 판단 */
	if (AbilityTagContainer.HasTag(AttackTag) || CurrentPlayerTag.MatchesTag(CrouchTag))
	{
		return;
	}
	const FVector2D MovementVector = InputValue.Get<FVector2D>();
	if ((MovementVector.Y < 0.0f && MovementVector.X != 0.0f))
	{
		CurrentPlayerTag = CrouchTag;
		bCheckTickCrouch = true;
		return;
	}
	
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
		bCheckTickCrouch = true;
	}

	FString MoveTag = "Idle";
	if (MovementVector.X > 0.0f)
	{
		if (bLookingRight)
		{
			MoveTag = "WalkForward";
		}
		else
		{
			MoveTag = "WalkBackward";
		}
	}
	else if (MovementVector.X < 0.0f)
	{
		if (bLookingRight)
		{
			MoveTag = "WalkBackward";
		}
		else
		{
			MoveTag = "WalkForward";
		}
	}
	
	CurrentPlayerTag = FGameplayTag::RequestGameplayTag(FName(*FString::Printf(TEXT("PlayerState.Base.%s.%s"), *CurrentStandTag, *MoveTag)));
}

void AFighter::StartJump(const FInputActionValue& InputValue)
{
	// 지상공격 캔슬 ( 밑에 태그 바꾸는것보다 먼저 호출해야됨 )
	if (UAnimInstance* AnimInst = GetMesh()->GetAnimInstance())
	{
		AnimInst->Montage_Stop(0.0f);
	}
	CurrentPlayerTag = JumpTag;
	Jump();
}

void AFighter::SetChangeStandTag()
{
	if (!AbilityTagContainer.HasTag(AttackTag))
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
}

void AFighter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	if (UAnimInstance* AnimInst = GetMesh()->GetAnimInstance())
	{
		AnimInst->Montage_Stop(0.0f);
	}

	if (CurrentPlayerTag.MatchesTag(HitTag))
	{
		if (CurrentPlayerTag == FGameplayTag::RequestGameplayTag(FName(TEXT("PlayerState.Base.Hit.Launch"))))
		{
			CurrentPlayerTag = FGameplayTag::RequestGameplayTag(FName(TEXT("PlayerState.Base.Hit.BounceBelly")));
		}
	}
	else
	{
		CurrentPlayerTag = FGameplayTag::RequestGameplayTag(FName(TEXT("PlayerState.Base.Land")));	
	}
}

FGameplayTagContainer& AFighter::GetCurrentTags()
{
	 return AbilityTagContainer; 
}

void AFighter::UnlockedTag()
{
	FString NextMontageName = GetGameInstance()->GetSubsystem<UAbilityManager>()->GetNextMontageName();
	if (NextMontageName.Len() < 1)
	{
		CurrentLockTag = FGameplayTag::RequestGameplayTag(FName("PlayerState.Attack"));	
	}
	else
	{
		if (NextMontageName.EndsWith(TEXT("stand"), ESearchCase::IgnoreCase))
		{
			CurrentPlayerTag = IdleTag;
		}
		else if (NextMontageName.EndsWith(TEXT("crouch"), ESearchCase::IgnoreCase))
		{
			CurrentPlayerTag = CrouchTag;
		}
	}
}

void AFighter::RefreshlockTag()
{
	if (CurrentMontageName.EndsWith(TEXT("stand"), ESearchCase::IgnoreCase))
	{
		CurrentPlayerTag = IdleTag;
	}
	else if (CurrentMontageName.EndsWith(TEXT("crouch"), ESearchCase::IgnoreCase))
	{
		CurrentPlayerTag = CrouchTag;
	}
}

void AFighter::LockTag()
{
	if (!CurrentPlayerTag.MatchesTag(JumpTag))
	{
		CurrentLockTag = CurrentPlayerTag;	
	}
}


void AFighter::ChangeLook()
{
	if (bLookingRight)
	{
		bLookingRight = false;
		SetActorRotation(FRotator(0.0f, 180.0f, 0.0f));
	}
	else
	{
		bLookingRight = true;
	}
}

void AFighter::SetCheckTickCrouch()
{
	bCheckTickCrouch = false;
}
