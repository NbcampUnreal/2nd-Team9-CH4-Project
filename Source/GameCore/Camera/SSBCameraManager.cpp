#include "GameCore/Camera/SSBCameraManager.h"
#include "GameCore/Camera/SSBCamera.h"
#include "EngineUtils.h"
#include "GameFramework/Actor.h"

void ASSBCameraManager::SetViewTarget(AActor* NewTarget, FViewTargetTransitionParams TransitionParams)
{

    Super::SetViewTarget(NewTarget, TransitionParams);
}

void ASSBCameraManager::UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime)
{
    if (PCOwner)
    {
        if (OutVT.Target)
        {
            if (!OutVT.Target->IsA(ASSBCamera::StaticClass()))
            {
                for (TActorIterator<ASSBCamera> It(PCOwner->GetWorld()); It; ++It)
                {
                    OutVT.Target = *It;
                    break;
                }
            }
        }
    }

    Super::UpdateViewTarget(OutVT, DeltaTime);
}