#pragma once

#include "CoreMinimal.h"
#include "GameplayTagAssetInterface.h"
#include "Components/ActorComponent.h"

#include "HitComponent.generated.h"


USTRUCT(BlueprintType)
struct FHitDamageAmount
{
	GENERATED_BODY()

	FHitDamageAmount(): HitDamageAmount(0), HitDamageAmountToShield(0), KnockbackAmount(0.0f)
	{
	}

	FHitDamageAmount(const int32 InHitDamageAmount,
	                 const int32 InHitDamageAmountToShield,
	                 const float InKnockbackAmount)
		: HitDamageAmount(InHitDamageAmount),
		  HitDamageAmountToShield(InHitDamageAmountToShield),
		  KnockbackAmount(InKnockbackAmount)
	{
	}

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 HitDamageAmount;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 HitDamageAmountToShield;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float KnockbackAmount;
};

USTRUCT(BlueprintType)
struct FHitDataInfo
{
	GENERATED_BODY()

	FHitDataInfo(): bIsRight(false),
	            HitDamageAmount(FHitDamageAmount()),
	            HitAngle(0.0f),
	            StopDuration(0.0f),
	            StunDuration(0.0f)
	{
	}

	FHitDataInfo(const bool bInIsRight,
	         const FHitDamageAmount& InHitDamageAmount,
	         const float InHitAngle,
	         const float InStopDuration,
	         const float InStunDuration)
		: bIsRight(bInIsRight),
		  HitDamageAmount(InHitDamageAmount),
		  HitAngle(InHitAngle),
		  StopDuration(InStopDuration),
		  StunDuration(InStunDuration)
	{
	}

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsRight;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FHitDamageAmount HitDamageAmount;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float HitAngle;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float StopDuration;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float StunDuration;
};

USTRUCT(BlueprintType)
struct FFighterKnockbackResult
{
	GENERATED_BODY()

	FFighterKnockbackResult(): bIsLaunch(false), bIsStillLaunched(false), LaunchDelay(0.0f)
	{
	}

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsLaunch;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsStillLaunched;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float LaunchDelay;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMECORE_API UHitComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHitComponent();

	UFUNCTION(Server, Reliable, BlueprintCallable, Category="HitComponents")
	void ServerHit(const FHitDataInfo& HitDataInfo);

protected:
	virtual void BeginPlay() override;

private:
	bool CanTakeDamage();
	void Hit(const FHitDamageAmount& HitDamageAmount, const float StopDuration);
	void StartHitStop(const float StopDuration);
	void EndHitStop() const;

	void ApplyKnockback(const bool IsRight, const float HitAngle, const float StunDuration);
	void FighterKnockback(FFighterKnockbackResult& OutFighterKnockbackResult, const bool IsRight, const float HitAngle);
	
	float CalculateKnockbackDistance();
	static FVector CalculateLaunchVector(const bool bIsRight, const float HitAngle, const float KnockbackDistance);

	void LaunchBounce();
	void FloorBounce();

	bool bIsHit;

	UPROPERTY(EditDefaultsOnly, meta=(ClampMin=500.0, ClampMax = 5000.0))
	float LaunchThreshold;
};
