#include "GameCore/Camera/SSBCamera.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "GameCore/Fighter/Fighter.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/PlayerController.h"

ASSBCamera::ASSBCamera()
{
    PrimaryActorTick.bCanEverTick = true;

    bReplicates = true;
    bAlwaysRelevant = true;
    SetReplicateMovement(true);

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    RootComponent = CameraBoom;

    CameraBoom->TargetArmLength = 3000.f;
    CameraBoom->bUsePawnControlRotation = false;
    CameraBoom->bDoCollisionTest = false;
    CameraBoom->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;
}

void ASSBCamera::BeginPlay()
{
    Super::BeginPlay();
    RefreshTrackedPlayers();
}

void ASSBCamera::SetStopped(bool bStop)
{
    bIsStopped = bStop;
}

void ASSBCamera::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    RefreshTrackedPlayers();
    UpdateCamera(DeltaTime);
}

void ASSBCamera::RefreshTrackedPlayers()
{
    TrackedPlayers.Empty();

    uint32 InsideCount = 0;
    for (TActorIterator<APawn> It(GetWorld()); It; ++It)
    {
        APawn* Pawn = *It;
        if (IsValid(Pawn))
        {
            if (const AFighter* Fighter = Cast<AFighter>(Pawn))
            {
                if (Fighter->GetIsInside())
                {
                    TrackedPlayers.Add(Pawn);
                    InsideCount++;    
                }
            }
        }
    }

    if (bIsStopped)
    {
        if (InsideCount >= 2)
        {
            bIsStopped = false;
        }
    }
}

void ASSBCamera::UpdateCamera(float DeltaTime)
{
    if (TrackedPlayers.Num() == 0 || bIsStopped)
        return;

    FVector MidPoint = FVector::ZeroVector;
    for (APawn* Player : TrackedPlayers)
    {
        if (Player)
        {
            MidPoint += Player->GetActorLocation();
        }
    }
    MidPoint /= TrackedPlayers.Num();

    SetActorLocation(FMath::VInterpTo(GetActorLocation(), MidPoint, DeltaTime, 5.f));

    float MaxDistance = 0.f;
    for (int32 i = 0; i < TrackedPlayers.Num(); ++i)
    {
        for (int32 j = i + 1; j < TrackedPlayers.Num(); ++j)
        {
            if (TrackedPlayers[i] && TrackedPlayers[j])
            {
                float Distance = FMath::Abs(TrackedPlayers[i]->GetActorLocation().X - TrackedPlayers[j]->GetActorLocation().X);
                MaxDistance = FMath::Max(MaxDistance, Distance);
            }
        }
    }
    float TargetZoom = FMath::Clamp(MaxDistance * 1.2f, MinZoomDistance, MaxZoomDistance);
    CameraBoom->TargetArmLength = FMath::FInterpTo(CameraBoom->TargetArmLength, TargetZoom, DeltaTime, ZoomInterpSpeed);
}

void ASSBCamera::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}