#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HitboxShape.h"
#include "HitBox.generated.h"

UCLASS()
class GAMECORE_API AHitBox : public AActor
{
	GENERATED_BODY()
	
public:	
	AHitBox();

protected:
    virtual void BeginPlay();
	virtual void OnConstruction(const FTransform& Transform) override;

public:	
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
    EHitboxShape HitboxShape;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
    FVector BoxExtent = FVector(50.f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
    float SphereRadius = 50.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
    float CapsuleRadius = 34.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
    float CapsuleHalfHeight = 88.f;

    UFUNCTION()
    void OnHitBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
    UPROPERTY()
    class UShapeComponent* CollisionComponent;

    void CreateHitboxShape();

};
