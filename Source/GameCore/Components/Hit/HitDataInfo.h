#pragma once

// USTRUCT(BlueprintType)
// struct FHitDirection
// {
// 	GENERATED_BODY()
//
// 	FHitDirection(): bIsRight(false), HitAngle(0.0f)
// 	{
// 	}
//
// 	FHitDirection(const bool bInIsRight, const float InHitAngle): bIsRight(bInIsRight), HitAngle(InHitAngle)
// 	{
// 	}
//
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite)
// 	bool bIsRight;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite)
// 	float HitAngle;
// };
//
// USTRUCT(BlueprintType)
// struct FHitDamageAmount
// {
// 	GENERATED_BODY()
//
// 	FHitDamageAmount(): HitDamageAmount(0.0f), HitDamageAmountToShield(0.0f), KnockbackAmount(0.0f)
// 	{
// 	}
//
// 	FHitDamageAmount(const float InHitDamageAmount,
// 					 const float InHitDamageAmountToShield,
// 					 const float InKnockbackAmount)
// 		: HitDamageAmount(InHitDamageAmount),
// 		  HitDamageAmountToShield(InHitDamageAmountToShield),
// 		  KnockbackAmount(InKnockbackAmount)
// 	{
// 	}
//
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite)
// 	float HitDamageAmount;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite)
// 	float HitDamageAmountToShield;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite)
// 	float KnockbackAmount;
// };
//
// USTRUCT(BlueprintType)
// struct FHitDataInfo
// {
// 	GENERATED_BODY()
//
// 	FHitDataInfo()
// 		: HitDirection(FHitDirection()),
// 		  HitDamageAmount(FHitDamageAmount()),
// 		  StopDuration(0.0f)
// 	{
// 	}
//
// 	FHitDataInfo(const FName InHitAbilityTagName,
// 				 const FHitDirection& InHitDirection,
// 				 const FHitDamageAmount& InHitDamageAmount,
// 				 const float InStopDuration)
// 		: HitAbilityTagName(InHitAbilityTagName),
// 		  HitDirection(InHitDirection),
// 		  HitDamageAmount(InHitDamageAmount),
// 		  StopDuration(InStopDuration)
// 	{
// 	}
//
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite)
// 	FName HitAbilityTagName;
//
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite)
// 	FHitDirection HitDirection;
//
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite)
// 	FHitDamageAmount HitDamageAmount;
//
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite)
// 	float StopDuration;
// };