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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsRight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HitAngle;
};

USTRUCT(BlueprintType)
struct FHitDamageAmount
{
	GENERATED_BODY()

	FHitDamageAmount(): HitDamageAmount(0.0f), HitDamageAmountToShield(0.0f), KnockbackAmount(0.0f)
	{
	}

	FHitDamageAmount(const float InHitDamageAmount,
	                 const float InHitDamageAmountToShield,
	                 const float InKnockbackAmount)
		: HitDamageAmount(InHitDamageAmount),
		  HitDamageAmountToShield(InHitDamageAmountToShield),
		  KnockbackAmount(InKnockbackAmount)
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HitDamageAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HitDamageAmountToShield;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName HitAbilityTagName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FHitDirection HitDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FHitDamageAmount HitDamageAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StopDuration;
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

	float CalculateKnockbackDistance(const float DamageScale, const float KnockbackAmount) const;
	static FVector CalculateLaunchVector(const FHitDirection& HitDirection, const float KnockbackDistance);

	float GetDamageScale(const FName InHitAbilityTagName) const;

	void FloorBounce();

	bool bIsHit;

	UPROPERTY(EditDefaultsOnly, meta=(ClampMin=500.0, ClampMax = 5000.0), Category="HitComponents")
	float LaunchThreshold;

	UPROPERTY(VisibleAnywhere, Category="HitComponents")
	FLastHitAbilityTagNameArray LastHitAbilityTagNameArray;
	UPROPERTY(EditDefaultsOnly, Category="HitComponents", meta=(ClampMin="0", ClampMax="9.0"))
	int32 MaxPenaltyCount;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="HitComponents", meta=(AllowPrivateAccess=true))
	float DamageAmplificationPercent;

	TSubclassOf<UUserWidget> UserWidgetClass;
};
