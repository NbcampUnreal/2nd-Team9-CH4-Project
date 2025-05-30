﻿#include "MapHitBox.h"

#include "NiagaraFunctionLibrary.h"
#include "GameCore/Camera/SSBCamera.h"
#include "GameCore/Fighter/Fighter.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/PlayerStart.h"
#include "Gameplay/Components/Input/PlayerInputComponent.h"
#include "Gameplay/PlayerController/SSBPlayerController.h"
#include "Gameplay/Components/Input/PlayerInputComponent.h"

AMapHitBox::AMapHitBox()
{
	PrimaryActorTick.bCanEverTick = false;

	HitBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("HitBoxComponent"));
	RootComponent = HitBoxComponent;

	HitBoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	HitBoxComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	HitBoxComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	HitBoxComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	//HitBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AMapHitBox::OnBeginOverlap);
	HitBoxComponent->OnComponentEndOverlap.AddDynamic(this, &AMapHitBox::OnEndOverlap);

	CameraHitBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CameraHitBoxComponent"));
	
	CameraHitBoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CameraHitBoxComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	CameraHitBoxComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CameraHitBoxComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	CameraHitBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AMapHitBox::OnBeginOverlap);
	CameraHitBoxComponent->OnComponentEndOverlap.AddDynamic(this, &AMapHitBox::OnCameraEndOverlap);

	bReplicates = true;
}

void AMapHitBox::BeginPlay()
{
	Super::BeginPlay();

	FallOffEffect->TranslucencySortPriority = 100;
}

void AMapHitBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AMapHitBox::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Cast<AFighter>(OtherActor)->SetIsInside(true);
}

void AMapHitBox::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	/* 플레이어 죽이기, 이펙트 호출, 리스폰은 타이머, 카메라 쉐이킹, 점수 증가 */
	if (!HasAuthority())
	{
		return;
	}
	
	AFighter* Fighter = Cast<AFighter>(OtherActor);
	if (!Fighter || GetWorld()->GetTimerManager().IsTimerActive(RespawnTimerHandle))
	{
		return;
	}

	/*FVector CamLoc = Fighter->GetController()->GetViewTarget()->GetActorLocation();
	CamLoc.Y = 0.f;
	const FVector DirToOrigin = (CamLoc - SpawnLoc).GetSafeNormal();*/
	const FVector SpawnLoc    = OtherActor->GetActorLocation() + FVector(0.0f, 100.f,0.0f);
	const FVector DirToOrigin = (FVector::ZeroVector - SpawnLoc).GetSafeNormal();
	const FQuat Quat = FQuat::FindBetweenNormals(FVector::UpVector, DirToOrigin);
	const FRotator SpawnRot = Quat.Rotator();
	SpawnEffect(SpawnLoc, SpawnRot);
	
	OnCameraShake();
	
	GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, [this, Fighter]()
	{
		if (!HasAuthority() || !Fighter)
		{
			return;
		}

		AController* PC = Fighter->GetController();
		if (!PC || !RespawnTarget)
		{
			return;
		}
		
		if (APawn* OldPawn = PC->GetPawn())
		{
			OldPawn->Destroy();
		}
		
		if (AGameModeBase* GM = GetWorld()->GetAuthGameMode())
		{
		//	GM->RestartPlayerAtPlayerStart(PC, RespawnTarget);
			if (APawn* NewPawn = GM->SpawnDefaultPawnFor(PC, RespawnTarget))
			{
				PC->Possess(NewPawn);
				//NewPawn->BeginPlay();
				/*if (ASSBPlayerController* SSBPC = Cast<ASSBPlayerController>(PC))
				{
					SSBPC->Client_OnRespawnedPawn();
				}#1#*/
			}
		}
	}, 3.0f, false);
}

void AMapHitBox::OnCameraEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (APlayerController* PC = It->Get())
		{
			ASSBCamera* Camera = Cast<ASSBCamera>(PC->GetViewTarget());

			if (Camera)
			{
				UE_LOG(LogTemp, Warning, TEXT("AMapHitBox::OnCameraEndOverlap"));

				Camera->SetStopped(true);

				Cast<AFighter>(OtherActor)->SetIsInside(false);
			}
		}
	}
}

void AMapHitBox::SpawnEffect(const FVector& SpawnLoc, const FRotator& SpawnRot) const
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (APlayerController* PC = It->Get())
		{
			if (ASSBPlayerController* SSBPC = Cast<ASSBPlayerController>(PC))
			{
				SSBPC->Multicast_SpawnEffect(FallOffEffect, SpawnLoc, SpawnRot);
			}
		}
	}
}

inline void AMapHitBox::OnCameraShake()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (APlayerController* PC = It->Get())
		{
			PC->ClientStartCameraShake(RespawnCameraShake);
		}
	}
}
