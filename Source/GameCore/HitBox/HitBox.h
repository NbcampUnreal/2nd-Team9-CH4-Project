#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HitBoxShape.h"
#include "GameCore/Components/Hit/HitComponent.h"
#include "GameCore/Ability/AbilityData/AnimRow.h"
#include "HitBox.generated.h"


UCLASS()
class GAMECORE_API AHitBox : public AActor
{
	GENERATED_BODY()

public:
	AHitBox();

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable, Category = "HitBox")
	void Init(const FHitDataInfo& HitData, const FVector& Pos, const FAnimRow AnimRow);

	UFUNCTION()
	void OnHitBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                     const FHitResult& SweepResult);

private:
	UPROPERTY()
	class UShapeComponent* CollisionComponent;

	void CreateHitBoxShape(const FAnimRow& InAnimInfo);

	void DebugDrawShape(const FAnimRow& AnimInfo) const;

	UPROPERTY(EditAnywhere, Category = "HitBox")
	bool bIsDebugMode;

	UPROPERTY(EditDefaultsOnly, Category = "HitBox")
	EHitBoxShape HitBoxShape;

	UPROPERTY(EditDefaultsOnly, Category = "HitBox")
	FVector BoxExtent = FVector(50.f);

	UPROPERTY(EditDefaultsOnly, Category = "HitBox")
	float SphereRadius = 50.f;

	UPROPERTY(EditDefaultsOnly, Category = "HitBox")
	float CapsuleRadius = 34.f;

	UPROPERTY(EditDefaultsOnly, Category = "HitBox")
	float CapsuleHalfHeight = 88.f;

	FHitDataInfo HitDataInfo;
	FAnimRow AnimInfo;
	
	UPROPERTY()
	UHitComponent* OwnerHitComponent;
};
