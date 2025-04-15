#include "HitBox.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameCore/Ability/AbilityManager/AbilityManager.h"
#include "GameCore/Fighter/Fighter.h"
#include "Kismet/KismetSystemLibrary.h"

AHitBox::AHitBox()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	bIsDebugMode = false;
}

void AHitBox::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
}

void AHitBox::BeginPlay()
{
	Super::BeginPlay();
}

void AHitBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetOwner())
	{
		/* 플레이어가 왼쪽 바라보고 있을땐 위치 반전 시켜서 적용시켜야함 */
		FTransform BoneTransform = Cast<AFighter>(GetOwner())->GetMesh()->GetSocketTransform(AnimInfo.BoneName, RTS_World);
		FVector BoneLocation =  BoneTransform.GetLocation();
		if (bIsMirrored)
		{
			FVector PlayerLocation = GetOwner()->GetActorLocation();
			FVector OffSet = BoneLocation - PlayerLocation;
			OffSet.X *= -1.f;
			BoneLocation = PlayerLocation + OffSet; 
		}
		SetActorLocation(BoneLocation);
	}
	if (bIsDebugMode)
	{
		DebugDrawShape(AnimInfo);
	}
}

void AHitBox::Init(const FHitDataInfo& HitData, const FVector& Pos, const FAnimRow AnimRow)
{
	HitDataInfo = HitData;
	AnimInfo = AnimRow;
	bIsMirrored = HitData.HitDirection.bIsRight;
	
	CreateHitBoxShape(AnimRow);
	
	if (CollisionComponent)
	{
		CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AHitBox::OnHitBoxOverlap);
	}
	
	//생성위치
	SetActorLocation(Pos);
	
	if (GetOwner())
	{
		if (const AFighter* OwnerFighter = Cast<AFighter>(GetOwner()))
		{
			if (UActorComponent* ActorComponent = OwnerFighter->GetComponentByClass(UHitComponent::StaticClass()))
			{
				if (UHitComponent* HitComponent = Cast<UHitComponent>(ActorComponent))
				{
					OwnerHitComponent = HitComponent;
					
				}
			}
		}
	}

	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AHitBox::CreateHitBoxShape(const FAnimRow& InAnimInfo)
{
	if (CollisionComponent)
	{
		CollisionComponent->DestroyComponent();
		CollisionComponent = nullptr;
	}
	HitBoxShape = static_cast<EHitBoxShape>(InAnimInfo.HitComType);
	switch (HitBoxShape)
	{
	case EHitBoxShape::Sphere:
		{
			USphereComponent* Sphere = NewObject<USphereComponent>(this, USphereComponent::StaticClass(),
			                                                       TEXT("SphereComponent"));
			if (InAnimInfo.Radius == 0)
			{
				Sphere->InitSphereRadius(SphereRadius);
			}
			else
			{
				SphereRadius = InAnimInfo.Radius;
				Sphere->InitSphereRadius(SphereRadius);
			}
			CollisionComponent = Sphere;
			break;
		}
	case EHitBoxShape::Box:
		{
			UBoxComponent* Box = NewObject<UBoxComponent>(this, UBoxComponent::StaticClass(), TEXT("BoxComponent"));
			
			BoxExtent = InAnimInfo.HitComScale;
			Box->InitBoxExtent(BoxExtent);
			CollisionComponent = Box;
			break;
		}
	case EHitBoxShape::Capsule:
		{
			UCapsuleComponent* Capsule = NewObject<UCapsuleComponent>(this, UCapsuleComponent::StaticClass(),
			                                                          TEXT("CapsuleComponent"));
			Capsule->InitCapsuleSize(CapsuleRadius, CapsuleHalfHeight);
			CollisionComponent = Capsule;
			break;
		}
	}

	if (CollisionComponent)
	{
		CollisionComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		CollisionComponent->RegisterComponent();
		CollisionComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
		CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AHitBox::DebugDrawShape(const FAnimRow& InAnimInfo) const
{
	switch (HitBoxShape)
	{
	case EHitBoxShape::Sphere:
		DrawDebugSphere(GetWorld(), GetActorLocation(), SphereRadius, 12, FColor::Red);
		break;
	case EHitBoxShape::Capsule:
		DrawDebugCapsule(GetWorld(), GetActorLocation(), CapsuleHalfHeight, CapsuleRadius, FQuat::Identity, FColor::Red);
		break;
	case EHitBoxShape::Box:
		DrawDebugBox(GetWorld(), GetActorLocation(), BoxExtent, FColor::Red);
		break;
	}
}

void AHitBox::OnHitBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                              UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                              const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this && OtherActor != GetOwner())
	{
		if (AFighter* OtherFighter = Cast<AFighter>(OtherActor))
		{
			if (UActorComponent* ActorComponent = OtherFighter->GetComponentByClass(UHitComponent::StaticClass()))
			{
				if (UHitComponent* HitComponent = Cast<UHitComponent>(ActorComponent))
				{
					HitDataInfo.HitDirection.bIsRight = GetGameInstance()->GetSubsystem<UAbilityManager>()->GetPlayerInstance()->GetPlayerLookingRight(); 
					SetOtherHit(OtherFighter);
					HitComponent->OnHit(OwnerHitComponent, HitDataInfo);
					Destroy();
				}
			}
		}
		// APawn* Pawn = Cast<APawn>(OtherActor);
		// if (Pawn)
		// {
		//     ACharacter* Character = Cast<ACharacter>(OtherActor);
		//     if (Character)
		//     {
		//         FVector LaunchVelocity = (Character->GetActorLocation() - GetActorLocation()).GetSafeNormal() * 1000.f;
		//         LaunchVelocity.Z = 500.f;
		//         Character->LaunchCharacter(LaunchVelocity, true, true);
		//     }
		//     else
		//     {
		//         UPrimitiveComponent* RootPrimitive = Cast<UPrimitiveComponent>(Pawn->GetRootComponent());
		//         if (RootPrimitive && RootPrimitive->IsSimulatingPhysics())
		//         {
		//             FVector LaunchDirection = Pawn->GetActorLocation() - GetActorLocation();
		//             LaunchDirection.Z = 0.5f;
		//             LaunchDirection.Normalize();
		//             RootPrimitive->AddImpulse(LaunchDirection * 50000.f);
		//         }
		//     }
		// }
	}
}

void AHitBox::SetOtherHit(AFighter* OtherFighter)
{
	
	if (HitDataInfo.HitDamageAmount.KnockbackAmount < 50)
	{
		OtherFighter->SetGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("PlayerState.Base.Hit.Weak")));	
	}
	else if (HitDataInfo.HitDamageAmount.KnockbackAmount < 100)
	{
		OtherFighter->SetGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("PlayerState.Base.Hit.Heavy")));
	}
	else
	{
		// 얼마나 크게 할건지 판단
		if (HitDataInfo.HitDirection.HitAngle > 0)
		{
			OtherFighter->SetGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("PlayerState.Base.Hit.Launch")));
			FGameplayTag A = OtherFighter->GetGameplayTag();
			UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Launch!!")));
		}
	}
}
