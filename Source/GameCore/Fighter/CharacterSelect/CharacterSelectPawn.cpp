#include "CharacterSelectPawn.h"

#include "GameplayTagContainer.h"

ACharacterSelectPawn::ACharacterSelectPawn()
{
	PrimaryActorTick.bCanEverTick = true;
	
	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(RootSceneComponent);

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMeshComponent->AttachToComponent(RootSceneComponent, FAttachmentTransformRules::KeepRelativeTransform);
}


void ACharacterSelectPawn::BeginPlay()
{
	Super::BeginPlay();
}

void ACharacterSelectPawn::MulticastChangeCharacterModel_Implementation(const FName CharacterTypeTagName)
{
	const FGameplayTag CharacterTypeTag = FGameplayTag::RequestGameplayTag(CharacterTypeTagName);

	if (CharacterModelDataArray.Contains(CharacterTypeTag))
	{
		auto [SkeletalMesh, MaterialArray, IdleAnimation, IconTexture] = CharacterModelDataArray[CharacterTypeTag];
		SkeletalMeshComponent->SetSkeletalMesh(SkeletalMesh);
		for (int32 i = 0; i < MaterialArray.Num(); i++)
		{
			SkeletalMeshComponent->SetMaterial(i, MaterialArray[i]);
		}

		SkeletalMeshComponent->PlayAnimation(IdleAnimation, true);

		OnChangeCharacter.Broadcast(IconTexture);
	}
}
