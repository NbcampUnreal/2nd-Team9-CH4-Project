#include "HitComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

UHitComponent::UHitComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	bIsHit = false;
	LaunchThreshold = 1000.0f;
}

void UHitComponent::ServerHit_Implementation(const FHitDataInfo& HitDataInfo)
{
	if (bIsHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Already Hitting"));
		return;
	}

	bIsHit = true;
}

void UHitComponent::BeginPlay()
{
	Super::BeginPlay();
	SetIsReplicated(true);
}

bool UHitComponent::CanTakeDamage()
{
	// TODO 게임플레이 태그로 저장된 HitState를 가져와서 확인

	// TODO 그 외 다양한 상태에서 공격을 받을 수 있는지 확인

	return true;
}

void UHitComponent::Hit(const FHitDamageAmount& HitDamageAmount, const float StopDuration)
{
	// TODO 현재 방어 중일 경우 처리 추가 -> HitState로 판단
	
	// TODO 화면 흔들림

	StartHitStop(StopDuration);
}

void UHitComponent::StartHitStop(const float StopDuration)
{
	if (GetOwner())
	{
		GetOwner()->CustomTimeDilation = 0.1f;

		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(
			TimerHandle,
			this,
			&UHitComponent::EndHitStop,
			StopDuration,
			false
		);
	}
}

void UHitComponent::EndHitStop() const
{
	if (GetOwner())
	{
		GetOwner()->CustomTimeDilation = 1.0f;
	}
}

void UHitComponent::ApplyKnockback(const bool IsRight, const float HitAngle, const float StunDuration)
{
	FFighterKnockbackResult FighterKnockbackResult;
	FighterKnockback(FighterKnockbackResult, IsRight, HitAngle);
}

void UHitComponent::FighterKnockback(FFighterKnockbackResult& OutFighterKnockbackResult, bool IsRight, float HitAngle)
{
	if (ACharacter* OwnerCharacter = GetOwner<ACharacter>())
	{
		if (UCharacterMovementComponent* CharacterMovementComponent = OwnerCharacter->GetCharacterMovement())
		{
			CharacterMovementComponent->SetMovementMode(MOVE_Falling);
			const float KnockbackDistance = CalculateKnockbackDistance();
			OutFighterKnockbackResult.LaunchDelay = KnockbackDistance / 2000.0f;

			const FVector NewLocation = OwnerCharacter->GetActorLocation() + FVector(0.0f, 0.0f, 20.0f);
			OwnerCharacter->SetActorLocation(NewLocation);

			if (KnockbackDistance > LaunchThreshold)
			{
				// TODO 캐릭터 상태 launch로 설정
			}
				
			const FVector LaunchVector = CalculateLaunchVector(IsRight, HitAngle, KnockbackDistance);
			OwnerCharacter->LaunchCharacter(LaunchVector, false, false);

			// TODO LaunchBounce
		}
	}
}

float UHitComponent::CalculateKnockbackDistance()
{
	// TODO 넉백 거리 구하는 계산
	return 10.0f;
}

FVector UHitComponent::CalculateLaunchVector(const bool bIsRight, const float HitAngle, const float KnockbackDistance)
{
	const FRotator HitAngleRotator = FRotator(0.0f, 0.0f, HitAngle);
	FVector LaunchVectorFromHitAngle = UKismetMathLibrary::GetUpVector(HitAngleRotator).GetSafeNormal();

	LaunchVectorFromHitAngle *= KnockbackDistance;
	
	const float LaunchDirection = bIsRight ? 1.0f : -1.0f;
	return FVector(LaunchVectorFromHitAngle.X, LaunchVectorFromHitAngle.Z * LaunchDirection, LaunchVectorFromHitAngle.Y);
}

void UHitComponent::LaunchBounce()
{
	if (ACharacter* OwnerCharacter = GetOwner<ACharacter>())
	{
		
	}
}

void UHitComponent::FloorBounce()
{
}
