﻿//#pragma optimize("",off)

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
#include "Net/UnrealNetwork.h"

FGameplayTag AFighter::AttackTag = FGameplayTag::RequestGameplayTag(TEXT("PlayerState.Attack"));
FGameplayTag AFighter::BaseTag = FGameplayTag::RequestGameplayTag(TEXT("PlayerState.Base"));
FGameplayTag AFighter::JumpTag = FGameplayTag::RequestGameplayTag(TEXT("PlayerState.Base.Jump"));
FGameplayTag AFighter::IdleTag = FGameplayTag::RequestGameplayTag(TEXT("PlayerState.Base.Stand.Idle"));
FGameplayTag AFighter::LandTag = FGameplayTag::RequestGameplayTag(TEXT("PlayerState.Base.Land"));
FGameplayTag AFighter::HitTag = FGameplayTag::RequestGameplayTag(TEXT("PlayerState.Base.Hit"));
FGameplayTag AFighter::CrouchTag = FGameplayTag::RequestGameplayTag(TEXT("PlayerState.Base.Crouch.Idle"));
FGameplayTag AFighter::BlockingTag = FGameplayTag::RequestGameplayTag(TEXT("PlayerState.Base.Stand.Block"));
FGameplayTag AFighter::WeakHitTag = FGameplayTag::RequestGameplayTag(TEXT("PlayerState.Base.Hit.Weak"));
FGameplayTag AFighter::HeavyHitTag = FGameplayTag::RequestGameplayTag(TEXT("PlayerState.Base.Hit.Heavy"));
FGameplayTag AFighter::LaunchHitTag = FGameplayTag::RequestGameplayTag(TEXT("PlayerState.Base.Hit.Launch"));

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
	
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
}

void AFighter::PlayMontageOnAllClients(UAnimMontage* Montage, float PlayRate, FName StartSection)
{
	if (HasAuthority())
	{
		Multicast_PlayMontage(Montage, PlayRate, StartSection);
	}
	else if (GetController() && GetController()->IsLocalController())
	{
		PlayAnimMontage(Montage, PlayRate, StartSection);
		Server_RequestPlayMontage(Montage, PlayRate, StartSection);
	}
}

void AFighter::Server_UpdatePlayerTag_Implementation(const FGameplayTag& NewTag)
{
	CurrentPlayerTag = NewTag;
	ForceNetUpdate();
}

void AFighter::Server_RequestPlayMontage_Implementation(UAnimMontage* Montage, float PlayRate,
                                                        FName StartSection)
{
	PlayAnimMontage(Montage, PlayRate, StartSection);
}

void AFighter::Multicast_PlayMontage_Implementation(UAnimMontage* Montage, float PlayRate, FName StartSection)
{
	PlayAnimMontage(Montage, PlayRate, StartSection);
}

void AFighter::Server_UpdatePlayerTagContainer_Implementation(const FGameplayTagContainer& NewTagContainer)
{
	AbilityTagContainer = NewTagContainer;
	ForceNetUpdate();
}

void AFighter::Server_UpdateStandTag_Implementation(const FString& NewStandTag)
{
	CurrentStandTag = NewStandTag;
	ForceNetUpdate();
}

void AFighter::Server_UpdateLockTag_Implementation(const FGameplayTag& NewTag)
{
	CurrentLockTag = NewTag;
	ForceNetUpdate();
}

void AFighter::Server_UpdateLookingRight_Implementation(const bool bInLookRight)
{
	bLookingRight = bInLookRight;
}

void AFighter::Server_UpdateTickCrouch_Implementation(const bool bInCheckTickCrouch)
{
	bCheckTickCrouch = bInCheckTickCrouch;
}

void AFighter::MoveStart(const FInputActionValue& InputActionValue)
{
	const FVector2D MovementVector = InputActionValue.Get<FVector2D>();
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
	Server_UpdatePlayerTag(CurrentPlayerTag);
}

void AFighter::MoveEnd()
{
	CurrentPlayerTag = IdleTag;
	Server_UpdatePlayerTag(CurrentPlayerTag);
}

void AFighter::BeginPlay()
{
	Super::BeginPlay();
	bReplicates = true;
	
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

	if (IsLocallyControlled())
	{
		GetGameInstance()->GetSubsystem<UAbilityManager>()->UpdateCharacter(this, Type);
		GetGameInstance()->GetSubsystem<UAbilityManager>()->InitializeManager();
	}
}

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

Test
if(GetActorLocation().X < 0)
{
	bLookingRight = true;
}
else
{
	bLookingRight = false;
	SetActorRotation(FRotator(0.0f, 180.0f, 0.0f));
}
*/
void AFighter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (!IsLocallyControlled())
	{
		const UCharacterMovementComponent* MoveComp = GetCharacterMovement();
		
		if (!bLookingRight)
		{
			SetActorRotation(FRotator(0.0f, 180.0f, 0.0f));
		}
		/*else
		{
			SetActorRotation(FRotator::ZeroRotator);
		}*/
		
		if (GetCharacterMovement()->IsFalling() && !CurrentPlayerTag.MatchesTag(LaunchHitTag))
		{
			CurrentPlayerTag = JumpTag;
		}
		else if (MoveComp->IsCrouching())
		{
			CurrentPlayerTag = CrouchTag;
		}
		/*else if (MoveComp->IsMovingOnGround())
		{
			float Xvel = MoveComp->Velocity.X;
			
			if (FMath::Abs(Xvel) <= KINDA_SMALL_NUMBER)
			{
				CurrentPlayerTag = IdleTag;
			}
			else
			{
				const bool bIsForward = (Xvel > 0.f &&  bLookingRight) || (Xvel < 0.f && !bLookingRight);
				if (bIsForward)
				{
					CurrentPlayerTag = FGameplayTag::RequestGameplayTag(TEXT("PlayerState.Base.Stand.WalkForward"));
				}
				else
				{
					CurrentPlayerTag = FGameplayTag::RequestGameplayTag(TEXT("PlayerState.Base.Stand.WalkBackward"));
				}
			}
		}*/
		
		return;
		/*if (!AbilityTagContainer.HasTag(AttackTag) && CurrentPlayerTag != JumpTag && CurrentPlayerTag != LandTag &&
		!CurrentPlayerTag.MatchesTag(HitTag) && !CurrentPlayerTag.MatchesTag(BlockingTag))
		{
			CurrentPlayerTag = FGameplayTag::RequestGameplayTag(FName(*FString::Printf(TEXT("PlayerState.Base.%s.Idle"), *CurrentStandTag)));
			//CurrentPlayerTag = FGameplayTag::RequestGameplayTag(FName(*FString::Printf(TEXT("PlayerState.Base.Stand.Idle"))));
		}*/
	}

	if (!bLookingRight)
	{
		SetActorRotation(FRotator(0.0f, 180.0f, 0.0f));
	}
	if (GetCharacterMovement()->IsFalling())
	{
		CurrentPlayerTag = JumpTag;
	}
	
	/*if (AbilityTagContainer.HasTag(AttackTag) && CurrentPlayerTag.MatchesTag(BlockingTag))
	{
		CurrentPlayerTag = IdleTag;
	}
	
	if (!AbilityTagContainer.HasTag(AttackTag) && !bCheckTickCrouch && CurrentPlayerTag.MatchesTag(CrouchTag))
	{
		bCheckTickCrouch = false;
		CurrentPlayerTag = IdleTag;
		CurrentStandTag = "Stand";
	}

	// 매프레임 액터 로테이션을 갱신해줌 왜 매프레임 0으로 초기화되는지 파악안됨 
	if (!bLookingRight)
	{
		SetActorRotation(FRotator(0.0f, 180.0f, 0.0f));
	}
	
	if (!AbilityTagContainer.HasTag(AttackTag) && CurrentPlayerTag != JumpTag && CurrentPlayerTag != LandTag &&
		!CurrentPlayerTag.MatchesTag(HitTag) && !CurrentPlayerTag.MatchesTag(BlockingTag))
	{
		CurrentPlayerTag = FGameplayTag::RequestGameplayTag(FName(*FString::Printf(TEXT("PlayerState.Base.%s.Idle"), *CurrentStandTag)));
		//CurrentPlayerTag = FGameplayTag::RequestGameplayTag(FName(*FString::Printf(TEXT("PlayerState.Base.Stand.Idle"))));
	}

	if (CurrentLockTag.MatchesTag(BaseTag) && !CurrentPlayerTag.MatchesTag(BlockingTag))
	{
		if (CurrentLockTag.MatchesTag(BlockingTag))
		{
			CurrentLockTag = AttackTag;
			return;
		}
		CurrentPlayerTag = CurrentLockTag;
	}*/

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
	if (AbilityTagContainer.HasTag(AttackTag) || bIsCrouched)
	{
		return;
	}

	const FVector2D In = InputValue.Get<FVector2D>();
	const float XValue = In.X * (bLookingRight == true ? 1.0f : -1.0f);
	if (Controller && FMath::Abs(XValue) > KINDA_SMALL_NUMBER)
	{
		AddMovementInput(GetActorForwardVector(), XValue);
	}
	
	/*const FVector2D MovementVector = InputValue.Get<FVector2D>();
	/* 이번 틱에서 크라우치를 시도했는지 체크하고 이동을 막기 위함 #1#
	if ((MovementVector.Y < 0.0f && MovementVector.X != 0.0f))
	{
		CurrentPlayerTag = CrouchTag;
		bCheckTickCrouch = true;
		return;
	}
	
	AddMovementInput(FVector(1.0f, 0.0f, 0.0f), MovementVector.X);

	/* 점프중에는 이동은 하지만 애니메이션은 처리 X #1#
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
	
	CurrentPlayerTag = FGameplayTag::RequestGameplayTag(FName(*FString::Printf(TEXT("PlayerState.Base.%s.%s"), *CurrentStandTag, *MoveTag)));*/
}

void AFighter::StartJump(const FInputActionValue& InputValue)
{
	if (GetGameInstance()->GetSubsystem<UAbilityManager>()->CheckCurrentPlayingMontage())
	{
		return;
	}
	// 지상공격 캔슬 ( 밑에 태그 바꾸는것보다 먼저 호출해야됨 )
	/*if (UAnimInstance* AnimInst = GetMesh()->GetAnimInstance())
	{
		AnimInst->Montage_Stop(0.0f);
	}*/
	CurrentPlayerTag = JumpTag;
	//Server_UpdatePlayerTag(CurrentPlayerTag);
	Jump();
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

void AFighter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AFighter, CurrentPlayerTag);
	DOREPLIFETIME(AFighter, AbilityTagContainer);
	DOREPLIFETIME(AFighter, CurrentLockTag);
	DOREPLIFETIME(AFighter, CurrentStandTag);
	DOREPLIFETIME(AFighter, bLookingRight);
	DOREPLIFETIME(AFighter, bCheckTickCrouch);
	DOREPLIFETIME(AFighter, bOnBuffering);
	DOREPLIFETIME(AFighter, bBlocking);
	DOREPLIFETIME(AFighter, CurrentMontageName);
	DOREPLIFETIME(AFighter,	HitStopVector);
}

FGameplayTagContainer& AFighter::GetCurrentTags()
{
	 return AbilityTagContainer; 
}

void AFighter::OnRep_CurrentPlayerTag()
{
	
}

void AFighter::UnlockedTag()
{
	FString NextMontageName = GetGameInstance()->GetSubsystem<UAbilityManager>()->GetNextMontageName();
	if (NextMontageName.Len() < 1)
	{
		CurrentLockTag = FGameplayTag::RequestGameplayTag(FName("PlayerState.Attack"));
		Server_UpdateLockTag(CurrentLockTag);
	}
	else
	{
		if (NextMontageName.EndsWith(TEXT("stand"), ESearchCase::IgnoreCase))
		{
			CurrentPlayerTag = IdleTag;
			Server_UpdatePlayerTag(CurrentPlayerTag);
		}
		else if (NextMontageName.EndsWith(TEXT("crouch"), ESearchCase::IgnoreCase))
		{
			CurrentPlayerTag = CrouchTag;
			Server_UpdatePlayerTag(CurrentPlayerTag);
		}
	}
}

void AFighter::SetGameplayTag(const FGameplayTag& GameplayTag)
{
	CurrentPlayerTag = GameplayTag;
	Server_UpdatePlayerTag(CurrentPlayerTag);
}

void AFighter::SetCurrentStandTag(const FString& InStandTag)
{
	if (InStandTag == TEXT("Stand"))
	{
		CurrentPlayerTag = IdleTag;
	}
	else
	{
		CurrentPlayerTag = CrouchTag;
	}
	Server_UpdatePlayerTag(CurrentPlayerTag);
	
	//CurrentStandTag = InStandTag;
	//Server_UpdateStandTag(CurrentStandTag);
}

void AFighter::RefreshlockTag()
{
	if (CurrentMontageName.EndsWith(TEXT("stand"), ESearchCase::IgnoreCase))
	{
		CurrentPlayerTag = IdleTag;
		Server_UpdatePlayerTag(CurrentPlayerTag);
	}
	else if (CurrentMontageName.EndsWith(TEXT("crouch"), ESearchCase::IgnoreCase))
	{
		CurrentPlayerTag = CrouchTag;
		Server_UpdatePlayerTag(CurrentPlayerTag);
	}
}

void AFighter::AddAttackTag()
{
	AbilityTagContainer.AddTag(AttackTag);
	Server_UpdatePlayerTagContainer(AbilityTagContainer);
}

void AFighter::RemoveAttackTag()
{
	AbilityTagContainer.RemoveTag(AttackTag);
	Server_UpdatePlayerTagContainer(AbilityTagContainer);
}

void AFighter::SetChangeBaseTag()
{
	CurrentPlayerTag = BaseTag;
	Server_UpdatePlayerTag(CurrentPlayerTag);
}

void AFighter::SetCrouch()
{
	Crouch();
	//CurrentPlayerTag = CrouchTag;
	//Server_UpdatePlayerTag(CurrentPlayerTag);
}

void AFighter::SetUnCrouch()
{
	UnCrouch();
	//CurrentPlayerTag = IdleTag;
	//Server_UpdatePlayerTag(CurrentPlayerTag);
}

void AFighter::LockTag()
{
	if (!CurrentPlayerTag.MatchesTag(JumpTag))
	{
		CurrentLockTag = CurrentPlayerTag;
		Server_UpdateLockTag(CurrentLockTag);
	}
}

bool AFighter::GetBuffering()
{
	return bOnBuffering;
}

void AFighter::HitStop(const FVector& LaunchVector, const FGameplayTag& InHitTag, const bool bIsAttacker)
{
	if (!bIsAttacker)
	{
		if (UCharacterMovementComponent* MovementCom = GetCharacterMovement())
		{
			/* 현재 맞으면 무조건 히트상태로 바꿈
			 * 엎어져있을때 공격이 들어가면 자동스탠드임
			 * 아예 무적을 주거나 하단공격판정을 추가해야될듯
			 */
			if (InHitTag.MatchesTag(WeakHitTag))
			{
				if (CurrentPlayerTag.MatchesTag(LaunchHitTag))
				{
					HitStopVector = {0.0f, 0.0f, 10.0f};
					StopMovementTimerSet(MovementCom, 0.1f);
				}
				else
				{
					CurrentPlayerTag = FGameplayTag();
					FTimerHandle ResetTagTimer;
					GetWorld()->GetTimerManager().SetTimer(ResetTagTimer, [this]()
						{
							CurrentPlayerTag = WeakHitTag;
							Server_UpdatePlayerTag(CurrentPlayerTag);
						}, 0.05f, false);
				}
			}
			else if (InHitTag.MatchesTag(HeavyHitTag))
			{
				if (CurrentPlayerTag.MatchesTag(LaunchHitTag))
				{
					HitStopVector = LaunchVector;
					//HitStopVector = {0.0f, 0.0f, 20.0f};
					StopMovementTimerSet(MovementCom, 0.1f);
				}
				else
				{
					CurrentPlayerTag = FGameplayTag();
					FTimerHandle ResetTagTimer;
					GetWorld()->GetTimerManager().SetTimer(ResetTagTimer, [this]()
						{
							CurrentPlayerTag = HeavyHitTag;
							Server_UpdatePlayerTag(CurrentPlayerTag);
						}, 0.05f, false);
				}
			
			}
			else if (InHitTag.MatchesTag(LaunchHitTag))
			{
				CurrentPlayerTag = LaunchHitTag;
				HitStopVector = LaunchVector;
				StopMovementTimerSet(MovementCom, 0.3f);
			}
		}
	}
}

void AFighter::AfterLaunchHitStop()
{
	if (UCharacterMovementComponent* MovementComp = GetCharacterMovement())
	{
		MovementComp->SetMovementMode(MOVE_Falling);
	}

	LaunchCharacter(HitStopVector, true, true);
}

void AFighter::AfterHitStop()
{
	if (UCharacterMovementComponent* MovementComp = GetCharacterMovement())
	{
		MovementComp->SetMovementMode(MOVE_Falling);
	}

	LaunchCharacter(HitStopVector, true, true);
}

void AFighter::StopMovementTimerSet(UCharacterMovementComponent* MovementCom, float Time)
{
	MovementCom->StopMovementImmediately();
	MovementCom->DisableMovement();
				
	GetWorld()->GetTimerManager().SetTimer(HitStopTimerHandle, this, &AFighter::AfterHitStop, Time, false);
}

void AFighter::StartBlocking(const FInputActionValue& InputActionValue)
{
	if (CurrentPlayerTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName(TEXT("PlayerState.Base.Stand")))))
	{
		CurrentPlayerTag = BlockingTag;
		Server_UpdatePlayerTag(CurrentPlayerTag);
	}
}

void AFighter::EndBlocking(const FInputActionValue& InputActionValue)
{
	if (CurrentPlayerTag.MatchesTag(BlockingTag))
	{
		CurrentPlayerTag = IdleTag;
		Server_UpdatePlayerTag(CurrentPlayerTag);
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
	
	Server_UpdateLookingRight(bLookingRight);
}

void AFighter::SetIdleTag()
{
	CurrentPlayerTag = IdleTag;
	Server_UpdatePlayerTag(CurrentPlayerTag);
}

void AFighter::SetCheckTickCrouch()
{
	bCheckTickCrouch = false;
}
