#include "HitBox.h"
#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"

AHitBox::AHitBox()
{
    PrimaryActorTick.bCanEverTick = false;
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
}

void AHitBox::BeginPlay()
{
    Super::BeginPlay();

    if (CollisionComponent)
    {
        CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AHitBox::OnHitBoxOverlap);
    }
}

void AHitBox::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);
    CreateHitboxShape();
}

void AHitBox::Init(const FHitDataInfo& HitData)
{
    HitDataInfo = HitData;
}

void AHitBox::CreateHitboxShape()
{
    if (CollisionComponent)
    {
        CollisionComponent->DestroyComponent();
        CollisionComponent = nullptr;
    }

    switch (HitboxShape)
    {
    case EHitboxShape::Sphere:
    {
        USphereComponent* Sphere = NewObject<USphereComponent>(this, USphereComponent::StaticClass(), TEXT("SphereComponent"));
        Sphere->InitSphereRadius(SphereRadius);
        CollisionComponent = Sphere;
        break;
    }
    case EHitboxShape::Box:
    {
        UBoxComponent* Box = NewObject<UBoxComponent>(this, UBoxComponent::StaticClass(), TEXT("BoxComponent"));
        Box->InitBoxExtent(BoxExtent);
        CollisionComponent = Box;
        break;
    }
    case EHitboxShape::Capsule:
    {
        UCapsuleComponent* Capsule = NewObject<UCapsuleComponent>(this, UCapsuleComponent::StaticClass(), TEXT("CapsuleComponent"));
        Capsule->InitCapsuleSize(CapsuleRadius, CapsuleHalfHeight);
        CollisionComponent = Capsule;
        break;
    }
    }

    if (CollisionComponent)
    {
        CollisionComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
        CollisionComponent->RegisterComponent();
        CollisionComponent->SetCollisionProfileName(TEXT("OverlapAll"));
    }
}

void AHitBox::OnHitBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor != this)
    {
        APawn* Pawn = Cast<APawn>(OtherActor);
        if (Pawn)
        {
            ACharacter* Character = Cast<ACharacter>(OtherActor);
            if (Character)
            {
                FVector LaunchVelocity = (Character->GetActorLocation() - GetActorLocation()).GetSafeNormal() * 1000.f;
                LaunchVelocity.Z = 500.f;
                Character->LaunchCharacter(LaunchVelocity, true, true);
            }
            else
            {
                UPrimitiveComponent* RootPrimitive = Cast<UPrimitiveComponent>(Pawn->GetRootComponent());
                if (RootPrimitive && RootPrimitive->IsSimulatingPhysics())
                {
                    FVector LaunchDirection = Pawn->GetActorLocation() - GetActorLocation();
                    LaunchDirection.Z = 0.5f;
                    LaunchDirection.Normalize();
                    RootPrimitive->AddImpulse(LaunchDirection * 50000.f);
                }
            }
        }
    }
}