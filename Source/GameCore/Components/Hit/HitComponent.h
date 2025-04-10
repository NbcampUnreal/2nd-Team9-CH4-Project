#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"

#include "HitComponent.generated.h"

class AFighter;

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
	
	UFUNCTION(BlueprintCallable, Category="HitComponents")
	void OnHit(UHitComponent* AttackerHitComponent, const FHitDataInfo& HitDataInfo);

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category="HitComponents")
	void MulticastHandleHit(float StopDuration, FName HitAbilityTagName);

	void StartHitStop(const float StopDuration);
	
	UFUNCTION(BlueprintCallable, Category="HitComponents")
	FORCEINLINE float GetDamageAmplificationPercent() const { return DamageAmplificationPercent; }

	UFUNCTION(BlueprintCallable, Category="HitComponents")
	FORCEINLINE void SetCurrentPlayerStateTag(const FGameplayTag& NewPlayerStateTag)
	{
		CurrentPlayerStateTag = NewPlayerStateTag;
	}

protected:
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

private:
	bool CanTakeDamage();
	
	void ApplyKnockback(const FVector& LaunchVector) const;
	void EndHitStop() const;

	float CalculateKnockbackDistance(const float KnockbackAmount) const;
	static FVector CalculateLaunchVector(const FHitDirection& HitDirection);

	float GetDamageScale(const FName InHitAbilityTagName) const;

	bool bIsHit;

	UPROPERTY(EditAnywhere, Category="HitComponents", meta=(ClampMin=500.0, ClampMax = 5000.0))
	float LaunchThreshold;
	UPROPERTY(EditAnywhere, Category="HitComponents", meta=(ClampMin="0", ClampMax="9.0"))
	int32 MaxPenaltyCount;
	
	UPROPERTY(EditAnywhere, category="HitComponents")
	FGameplayTagContainer ShieldTags;
	
	UPROPERTY(VisibleAnywhere, category="HitComponents")
	FGameplayTag CurrentPlayerStateTag;
	UPROPERTY(VisibleAnywhere, Category="HitComponents")
	FLastHitAbilityTagNameArray LastHitAbilityTagNameArray;

	UPROPERTY(Replicated, VisibleAnywhere, Category="HitComponents")
	float DamageAmplificationPercent;

	TSubclassOf<UUserWidget> UserWidgetClass;

};
