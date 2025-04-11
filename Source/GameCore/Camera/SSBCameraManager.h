#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "SSBCameraManager.generated.h"

UCLASS()
class GAMECORE_API ASSBCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

public:
	virtual void SetViewTarget(AActor* NewTarget, FViewTargetTransitionParams TransitionParams = FViewTargetTransitionParams()) override;

	virtual void UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime) override;
};
