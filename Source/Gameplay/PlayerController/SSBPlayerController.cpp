#include "SSBPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "GameCore/Camera/SSBCameraManager.h"
#include "GameCore/Camera/SSBCamera.h"
#include "GameCore/Fighter/Fighter.h"
#include "Gameplay/Components/Input/PlayerInputComponent.h"
#include "MessageBus/CheatManager/SSBCheatManager.h"

ASSBPlayerController::ASSBPlayerController()
{
	PlayerInputComponent = CreateDefaultSubobject<UPlayerInputComponent>(TEXT("PlayerInputComponent"));
	CheatClass = USSBCheatManager::StaticClass();
    PlayerCameraManagerClass = ASSBCameraManager::StaticClass();
}

void ASSBPlayerController::BeginPlay()
{
	Super::BeginPlay();

    if (IsLocalController())
    {
        UWorld* World = GetWorld();
        if (World)
        {
            for (TActorIterator<ASSBCamera> It(World); It; ++It)
            {
                ASSBCamera* Camera = *It;
                if (Camera)
                {
                    SetViewTarget(Camera);
                    return;
                }
            }
        }
    }
}

void ASSBPlayerController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    if (IsLocalController())
    {
        UWorld* World = GetWorld();
        if (World)
        {
            for (TActorIterator<ASSBCamera> It(World); It; ++It)
            {
                ASSBCamera* Camera = *It;
                if (Camera)
                {
                    SetViewTarget(Camera);
                    UE_LOG(LogTemp, Warning, TEXT("[ASSBPlayerController] OnPossess: SetViewTarget to SSBCamera: %s"), *Camera->GetName());
                    return;
                }
            }
        }
    }
}

void ASSBPlayerController::PreProcessInput(const float DeltaTime, const bool bGamePaused)
{
    Super::PreProcessInput(DeltaTime, bGamePaused);
    if (IsValid(PlayerInputComponent->GetFighter()))
    {
        PlayerInputComponent->GetFighter()->SetCheckTickCrouch();
    }
}
