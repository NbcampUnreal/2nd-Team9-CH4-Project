#include "HitComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

UHitComponent::UHitComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	bIsHit = false;
	LaunchThreshold = 1000.0f;
	AccumulatedDamage = 0;

	DamageScales = {
		1.0f,
		0.9f,
		0.8f,
		0.7f,
		0.6f,
		0.5f
	};
}

void UHitComponent::ServerHit_Implementation(const FHitDataInfo& HitDataInfo)
{
	if (bIsHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Already Hitting"));
		return;
	}

	bIsHit = true;

	if (CanTakeDamage())
	{
		Hit(HitDataInfo.HitDamageAmount, HitDataInfo.StopDuration);

		ApplyKnockback(HitDataInfo);

		bIsHit = false;
	}
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

		if (StopDuration > 0.0f)
		{
			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(
				TimerHandle,
				this,
				&UHitComponent::EndHitStop,
				StopDuration,
				false
			);
		}
		else
		{
			EndHitStop();
		}
	}
}

void UHitComponent::EndHitStop() const
{
	if (GetOwner())
	{
		GetOwner()->CustomTimeDilation = 1.0f;
	}
}

void UHitComponent::ApplyKnockback(const FHitDataInfo& HitDataInfo)
{
	if (ACharacter* OwnerCharacter = GetOwner<ACharacter>())
	{
		if (UCharacterMovementComponent* CharacterMovementComponent = OwnerCharacter->GetCharacterMovement())
		{
			CharacterMovementComponent->SetMovementMode(MOVE_Falling);

			const float DamageScale = GetDamageScale(HitDataInfo.HitAbilityTagName);
			const float KnockbackDistance = CalculateKnockbackDistance(DamageScale, HitDataInfo.HitDamageAmount);

			if (KnockbackDistance > LaunchThreshold)
			{
				// TODO 캐릭터 상태 launch로 설정
			}

			const FVector LaunchVector = CalculateLaunchVector(HitDataInfo.HitDirection.HitAngle, KnockbackDistance);
			const bool bIsFalling = CharacterMovementComponent->IsFalling();
			OwnerCharacter->LaunchCharacter(LaunchVector, bIsFalling, bIsFalling);

			// TODO FloorBounce

			LastHitAbilityTagNameArray.AddHitAbilityTagName(HitDataInfo.HitAbilityTagName);
			AccumulatedDamage += HitDataInfo.HitDamageAmount.HitDamageAmount;
		}
	}
}

float UHitComponent::CalculateKnockbackDistance(const float DamageScale, const FHitDamageAmount& HitDamageAmount) const
{
	float KnockbackDistance = HitDamageAmount.KnockbackAmount;

	KnockbackDistance += DamageScale * (AccumulatedDamage * (2 + HitDamageAmount.HitDamageAmount)) / 20.0f;

	UKismetSystemLibrary::PrintString(GetOwner(), FString::Printf(TEXT("Knockback Distance: %.2f"), KnockbackDistance));

	return KnockbackDistance;
}

FVector UHitComponent::CalculateLaunchVector(const float HitAngle, const float KnockbackDistance)
{
	const FRotator HitAngleRotator = FRotator(0.0f, 0.0f, FMath::Abs(180 - HitAngle));
	FVector LaunchVectorFromHitAngle = UKismetMathLibrary::GetRightVector(HitAngleRotator).GetSafeNormal();

	LaunchVectorFromHitAngle *= KnockbackDistance;

	return LaunchVectorFromHitAngle;
}

float UHitComponent::GetDamageScale(const FName InHitAbilityTagName)
{
	// One Pattern Penalty
	int32 PenaltyCount = 0;
	for (const FName HitAbilityTagName : LastHitAbilityTagNameArray.GetLastHitAbilityTageNames())
	{
		if (HitAbilityTagName == InHitAbilityTagName)
		{
			PenaltyCount++;
		}
	}

	PenaltyCount = FMath::Min(PenaltyCount, DamageScales.Num() - 1);

	UKismetSystemLibrary::PrintString(GetOwner(), FString::Printf(TEXT("PenaltyCount: %d"), PenaltyCount));

	return DamageScales[PenaltyCount];
}

void UHitComponent::FloorBounce()
{
}
