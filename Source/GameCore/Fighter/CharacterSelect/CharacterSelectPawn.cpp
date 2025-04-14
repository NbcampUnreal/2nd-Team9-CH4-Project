#include "CharacterSelectPawn.h"

#include "GameCore/CharacterModelData/CharacterModelDataAsset.h"

ACharacterSelectPawn::ACharacterSelectPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(RootSceneComponent);
	RootSceneComponent->SetIsReplicated(true);

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMeshComponent->AttachToComponent(RootSceneComponent, FAttachmentTransformRules::KeepRelativeTransform);
	SkeletalMeshComponent->SetIsReplicated(true);
}


void ACharacterSelectPawn::InitLocation(const int32 OffsetY)
{
	SetActorLocation(FVector(0.0f, OffsetY, 0.0f));
}

void ACharacterSelectPawn::MulticastChangeCharacterModel_Implementation(
	const FCharacterModelData& CharacterModelData) const
{
	if (IsValid(SkeletalMeshComponent))
	{
		SkeletalMeshComponent->SetSkeletalMesh(CharacterModelData.SkeletalMesh);

		for (int32 i = 0; i < CharacterModelData.MaterialArray.Num(); i++)
		{
			SkeletalMeshComponent->SetMaterial(i, CharacterModelData.MaterialArray[i]);
		}
		
		SkeletalMeshComponent->PlayAnimation(CharacterModelData.IdleAnimation, true);
	}
}

void ACharacterSelectPawn::BeginPlay()
{
	Super::BeginPlay();
}
