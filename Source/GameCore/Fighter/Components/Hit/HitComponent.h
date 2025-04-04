#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "HitComponent.generated.h"

USTRUCT(BlueprintType)
struct FHitDirection
{
	GENERATED_BODY()

	FHitDirection(): bIsRight(false), HitAngle(0.0f)
	{
	}

	FHitDirection(const bool bInIsRight, const float InHitAngle): bIsRight(bInIsRight), HitAngle(InHitAngle)
	{
	}

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsRight;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float HitAngle;
};

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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 HitDamageAmount;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 HitDamageAmountToShield;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float KnockbackAmount;
};

USTRUCT(BlueprintType)
struct FHitDataInfo
{
	GENERATED_BODY()

	FHitDataInfo()
		: HitDirection(FHitDirection()),
		  HitDamageAmount(FHitDamageAmount()),
		  StopDuration(0.0f)
	{
	}

	FHitDataInfo(const FName InHitAbilityTagName,
	             const FHitDirection& InHitDirection,
	             const FHitDamageAmount& InHitDamageAmount,
	             const float InStopDuration)
		: HitAbilityTagName(InHitAbilityTagName),
		  HitDirection(InHitDirection),
		  HitDamageAmount(InHitDamageAmount),
		  StopDuration(InStopDuration)
	{
	}

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FName HitAbilityTagName;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FHitDirection HitDirection;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FHitDamageAmount HitDamageAmount;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float StopDuration;
};

USTRUCT(BlueprintType)
struct FFighterKnockbackResult
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsLaunch;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsStillLaunched;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float LaunchDelay;
};

USTRUCT()
struct FLastHitAbilityTagNameArray
{
	GENERATED_BODY()

	void AddHitAbilityTagName(const FName HitAbilityTagName)
	{
		if (InfoArray.Num() == 9)
		{
			InfoArray.RemoveAt(0);
		}

		InfoArray.Add(HitAbilityTagName);
	}

	TArray<FName> GetLastHitAbilityTageNames() const { return InfoArray; }

private:
	UPROPERTY(visibleAnywhere)
	TArray<FName> InfoArray;
};

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
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

	void ApplyKnockback(const FHitDataInfo& HitDataInfo);

	float CalculateKnockbackDistance(float DamageScale, const FHitDamageAmount& HitDamageAmount) const;
	static FVector CalculateLaunchVector(float HitAngle, const float KnockbackDistance);

	float GetDamageScale(const FName InHitAbilityTagName);

	void FloorBounce();

	bool bIsHit;

	UPROPERTY(EditDefaultsOnly, meta=(ClampMin=500.0, ClampMax = 5000.0), Category="HitComponents")
	float LaunchThreshold;

	UPROPERTY(VisibleAnywhere, Category="HitComponents")
	FLastHitAbilityTagNameArray LastHitAbilityTagNameArray;
	UPROPERTY(EditDefaultsOnly, Category="HitComponents")
	TArray<float> DamageScales;
	UPROPERTY(EditAnywhere, Category="HitComponents")
	int32 AccumulatedDamage;
};
