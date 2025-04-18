﻿#include "SSBPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "NiagaraFunctionLibrary.h"
#include "GameCore/Ability/AbilityManager/AbilityManager.h"
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
                }
            }
        }

        Client_OnRespawnedPawn();
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

void ASSBPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    if (IsLocalController())
    {
    //    PlayerInputComponent->BindActions(this);
    }
    
}

void ASSBPlayerController::OnRep_Pawn()
{
    Super::OnRep_Pawn();

    if (AFighter* F = Cast<AFighter>(GetPawn()))
    {
        PlayerInputComponent->SetFighter(F);
        PlayerInputComponent->BindActions(this);
        GetGameInstance()->GetSubsystem<UAbilityManager>()->UpdateCharacter(F, F->GetPlayerType());    
    }
}

void ASSBPlayerController::Multicast_SpawnEffect_Implementation(UNiagaraSystem* Effect, const FVector& SpawnLoc,
                                                                const FRotator& SpawnRot)
{
    if (Effect)
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),Effect,SpawnLoc,SpawnRot,
            FVector(0.6), true, true);
    }
}

void ASSBPlayerController::TryBindRespawnedPawn()
{
    if (AFighter* F = Cast<AFighter>(GetPawn()))
    {
        PlayerInputComponent->SetFighter(F);
        PlayerInputComponent->BindActions(this);
        GetGameInstance()->GetSubsystem<UAbilityManager>()->UpdateCharacter(F, F->GetPlayerType());
        
        GetWorld()->GetTimerManager().ClearTimer(RespawnPawnCheckTimer);
    }
}

void ASSBPlayerController::Client_OnRespawnedPawn_Implementation()
{
    if (AFighter* F = Cast<AFighter>(GetPawn()))
    {
        PlayerInputComponent->SetFighter(F);
        PlayerInputComponent->BindActions(this);
        GetGameInstance()->GetSubsystem<UAbilityManager>()->UpdateCharacter(F, F->GetPlayerType());
        return;
    }
    
    GetWorld()->GetTimerManager().SetTimer(RespawnPawnCheckTimer,this,&ASSBPlayerController::TryBindRespawnedPawn,
        0.1f,
        true 
    );
}

void ASSBPlayerController::Client_OnPossess_Implementation()
{
    if (AFighter* F = Cast<AFighter>(GetPawn()))
    {
        PlayerInputComponent->SetFighter(F);
    }

    
    PlayerInputComponent->FindFighter();
}
