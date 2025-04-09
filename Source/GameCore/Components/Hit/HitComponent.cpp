#include "HitComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"

UHitComponent::UHitComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);

	bIsHit = false;
	LaunchThreshold = 1000.0f;
	MaxPenaltyCount = 5;
	DamageAmplificationPercent = 0.0f;
}

void UHitComponent::OnHit(UHitComponent* AttackerHitComponent, const FHitDataInfo& HitDataInfo)
{
	if (!IsValid(GetOwner()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Owner Is Not Valid"));
		return;
	}

	if (!GetOwner()->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Not Run On The Server!"));
		return;
	}

	if (bIsHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Already Hitting"));
		return;
	}

	bIsHit = true;

	if (CanTakeDamage())
	{
		// Attacker Slowing
		if (IsValid(AttackerHitComponent))
		{
			AttackerHitComponent->MulticastHandleHit(0.05f, FName());
		}

		const float DamageScale = GetDamageScale(HitDataInfo.HitAbilityTagName);
		float KnockbackAmount = DamageScale * HitDataInfo.HitDamageAmount.KnockbackAmount;
		float HitDamageAmount = DamageScale * HitDataInfo.HitDamageAmount.HitDamageAmount;

		if (ShieldTags.HasTag(CurrentPlayerStateTag))
		{
			UKismetSystemLibrary::PrintString(GetOwner(), FString::Printf(TEXT("Shield!!")));

			KnockbackAmount *= 0.1f;
			HitDamageAmount = DamageScale * HitDataInfo.HitDamageAmount.HitDamageAmountToShield;

			// TODO 실드 차감 처리
		}

		const float KnockbackDistance = CalculateKnockbackDistance(KnockbackAmount);
		if (KnockbackDistance > LaunchThreshold)
		{
			// TODO 캐릭터 상태 launch로 설정
		}

		// TODO 아래방향 공격이 들어올때 처리 -> 조건에 따라 아래로 런치 or 다른 공격과 마찬가지로 런치

		const FVector LaunchVector = CalculateLaunchVector(HitDataInfo.HitDirection) * KnockbackDistance;
		ApplyKnockback(LaunchVector);
		MulticastHandleHit(HitDataInfo.StopDuration, HitDataInfo.HitAbilityTagName);

		DamageAmplificationPercent += HitDamageAmount;
	}

	bIsHit = false;
}

void UHitComponent::MulticastHandleHit_Implementation(const float StopDuration, const FName HitAbilityTagName)
{
	// Attacked Target Slowing
	StartHitStop(StopDuration);

	if (!HitAbilityTagName.IsNone())
	{
		LastHitAbilityTagNameArray.AddHitAbilityTagName(HitAbilityTagName);
	}
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

void UHitComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UHitComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UHitComponent, DamageAmplificationPercent);
}

bool UHitComponent::CanTakeDamage()
{
	// TODO 게임플레이 태그로 저장된 HitState를 가져와서 확인

	// TODO 그 외 다양한 상태에서 공격을 받을 수 있는지 확인

	return true;
}

void UHitComponent::EndHitStop() const
{
	if (GetOwner())
	{
		GetOwner()->CustomTimeDilation = 1.0f;
	}
}

void UHitComponent::ApplyKnockback(const FVector& LaunchVector) const
{
	if (ACharacter* OwnerCharacter = GetOwner<ACharacter>())
	{
		if (UCharacterMovementComponent* CharacterMovementComponent = OwnerCharacter->GetCharacterMovement())
		{
			CharacterMovementComponent->SetMovementMode(MOVE_Falling);

			const bool bIsFalling = CharacterMovementComponent->IsFalling();
			OwnerCharacter->LaunchCharacter(LaunchVector, bIsFalling, bIsFalling);

			// TODO FloorBounce
		}
	}
}

float UHitComponent::CalculateKnockbackDistance(const float KnockbackAmount) const
{
	const float DamageAmplification = DamageAmplificationPercent / 100.0f + 1.0f;
	const float KnockbackDistance = KnockbackAmount * DamageAmplification;

	return KnockbackDistance;
}

FVector UHitComponent::CalculateLaunchVector(const FHitDirection& HitDirection)
{
	const float KnockbackDirection = HitDirection.bIsRight ? -1.0f : 1.0f;
	const FRotator HitAngleRotator = FRotator((90.0f - HitDirection.HitAngle) * KnockbackDirection, 0.0f, 0.0f);

	return UKismetMathLibrary::GetUpVector(HitAngleRotator).GetSafeNormal();
}

float UHitComponent::GetDamageScale(const FName InHitAbilityTagName) const
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

	PenaltyCount = FMath::Min(PenaltyCount, MaxPenaltyCount);

	UKismetSystemLibrary::PrintString(GetOwner(), FString::Printf(TEXT("PenaltyCount: %d"), PenaltyCount));

	return PenaltyCount == 0.0f ? 1.0f : FMath::Pow(0.88f, PenaltyCount);
}
