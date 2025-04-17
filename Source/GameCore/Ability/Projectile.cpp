#include "Projectile.h"

#include "EffectType.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "GameCore/Fighter/Fighter.h"


AProjectile::AProjectile()
{
 	PrimaryActorTick.bCanEverTick = true;
}


void AProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	SetActorLocation(GetActorLocation()+(VecDir * Speed * DeltaTime));

	
	if (NiagaraComponent.Find(static_cast<int32>(AnimInfo.EffectType)))
	{
		UNiagaraComponent** ComPtr = NiagaraComponent.Find(static_cast<int32>(AnimInfo.EffectType));
		UNiagaraComponent* NiagaraCom = *ComPtr;
		bool bEnd = NiagaraCom->IsComplete();
		UE_LOG(LogTemp, Warning, TEXT("NiagaraComponent IsComplete: %s"), bEnd ? TEXT("true") : TEXT("false"));
		if (bEnd)
		{
			OnNiagaraFinished();
		}
	}
}

void AProjectile::Init(const FAnimRow AnimRow)
{
	AnimInfo = AnimRow;
	if (GetOwner())
	{
		bLookingRight = Cast<AFighter>(GetOwner())->GetPlayerLookingRight();
	}
	SetLocation();
	
	if (NiagaraEffect[static_cast<int32>(AnimInfo.EffectType)]->IsValid())
	{
		if (AnimInfo.EffectType == EEffectType::IceWall)
		{
			NiagaraComponent.Add(static_cast<int32>(EEffectType::IceWall), UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			NiagaraEffect[static_cast<int32>(EEffectType::IceWall)], // Niagara 시스템
			RootComponent->GetComponentLocation(),                     // 위치
			RootComponent->GetComponentRotation(),                     // 회전
			FVector(1.0f),                                         // 스케일
			false                                                      // 프리딕션 가능
			));
		}
		else
		{
			if (AnimInfo.EffectType == EEffectType::Gravity)
			{
				
				NiagaraComponent.Add(static_cast<int32>(EEffectType::Attacked), UNiagaraFunctionLibrary::SpawnSystemAttached(
				NiagaraEffect[static_cast<int32>(EEffectType::Attacked)],             // 이펙트 에셋
				RootComponent,          // 루트 컴포넌트
				NAME_None,                 // 소켓
				FVector::ZeroVector,       // 상대 위치
				FRotator::ZeroRotator,     // 회전
				EAttachLocation::KeepRelativeOffset,
				false                       // 자동 파괴
				));
			}
		
			NiagaraComponent.Add(static_cast<int32>(AnimInfo.EffectType), UNiagaraFunctionLibrary::SpawnSystemAttached(
			NiagaraEffect[static_cast<int32>(AnimInfo.EffectType)],             // 이펙트 에셋
			RootComponent,          // 루트 컴포넌트
			NAME_None,                 // 소켓
			FVector::ZeroVector,       // 상대 위치
			FRotator::ZeroRotator,     // 회전
			EAttachLocation::KeepRelativeOffset,
			false                       // 자동 파괴
			));
		}
		
	}
}

void AProjectile::SetLocation()
{
	if (GetOwner())
	{
		FVector PlayerLocation = GetOwner()->GetActorLocation();
		if (AnimInfo.EffectType == EEffectType::Bang)
		{
			VecDir = FVector(1.f, 0, 0.f);
		}
		else if (AnimInfo.EffectType == EEffectType::IceWall)
		{
			VecDir = FVector(0.f, 0, 1.f);
		}
		else
		{
			VecDir = FVector::ZeroVector;
		}
		if (!bLookingRight)
		{
			AnimInfo.HitComOffSet.X *= -1.f;
			if (AnimInfo.EffectType != EEffectType::IceWall)
			{
				VecDir *= -1;
			}
		}
		SetActorLocation(PlayerLocation + AnimInfo.HitComOffSet);
	}
}

void AProjectile::OnNiagaraFinished()
{
	for (auto& NiaEffect : NiagaraComponent)
	{
		if (IsValid(NiaEffect.Value))
		{
			NiaEffect.Value->DestroyComponent();
		}
	}
	NiagaraComponent.Empty();

	Destroy();
}
