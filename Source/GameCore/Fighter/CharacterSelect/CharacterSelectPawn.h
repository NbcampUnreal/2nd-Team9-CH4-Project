#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Pawn.h"
#include "CharacterSelectPawn.generated.h"

USTRUCT()
struct FCharacterModelData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	USkeletalMesh* SkeletalMesh;
	UPROPERTY(EditDefaultsOnly)
	TArray<UMaterialInterface*> MaterialArray;
	UPROPERTY(EditDefaultsOnly)
	UAnimationAsset* IdleAnimation;
};

UCLASS()
class GAMECORE_API ACharacterSelectPawn : public APawn
{
	GENERATED_BODY()

public:
	ACharacterSelectPawn();
	
	UFUNCTION(NetMulticast, Reliable)
	void MulticastChangeCharacterModel(const FName CharacterTypeTagName);
	
protected:
	virtual void BeginPlay() override;
	
private:
	void InitRenderTarget();
	
	UPROPERTY(VisibleAnywhere)
	USceneComponent* RootSceneComponent;
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* SkeletalMeshComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = "CharacterModelData")
	TMap<FGameplayTag, FCharacterModelData> CharacterModelDataArray;
	
};
