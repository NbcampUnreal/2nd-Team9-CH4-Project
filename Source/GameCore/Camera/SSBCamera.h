#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "SSBCamera.generated.h"

UCLASS()
class GAMECORE_API ASSBCamera : public AActor
{
    GENERATED_BODY()

public:
    ASSBCamera();

    virtual void Tick(float DeltaTime) override;

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
    virtual void BeginPlay() override;

public:
    void SetStopped(bool bStop);
private:
    UPROPERTY(VisibleAnywhere, Category = "Camera")
    USpringArmComponent* CameraBoom;

    UPROPERTY(VisibleAnywhere, Category = "Camera")
    UCameraComponent* FollowCamera;

    UPROPERTY(EditAnywhere, Category = "Camera|Settings")
    float MinZoomDistance = 1000.f;

    UPROPERTY(EditAnywhere, Category = "Camera|Settings")
    float MaxZoomDistance = 4000.f;

    UPROPERTY(EditAnywhere, Category = "Camera|Settings")
    float ZoomInterpSpeed = 5.f;

    UPROPERTY()
    TArray<APawn*> TrackedPlayers;

    bool bIsStopped = false;
    void UpdateCamera(float DeltaTime);
    void RefreshTrackedPlayers();
};