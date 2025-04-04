#include "HitBox.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"

AHitBox::AHitBox()
{
    PrimaryActorTick.bCanEverTick = false;
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
}

void AHitBox::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);
    CreateHitboxShape();
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