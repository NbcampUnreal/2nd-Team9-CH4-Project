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
	UPROPERTY(EditDefaultsOnly)
	UTexture2D* IconTexture;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangeCharacter, UTexture2D*, IconTexture);

UCLASS()
class GAMECORE_API ACharacterSelectPawn : public APawn
{
	GENERATED_BODY()

public:
	ACharacterSelectPawn();
	
	UFUNCTION(NetMulticast, Reliable)
	void MulticastChangeCharacterModel(const FName CharacterTypeTagName);
	
	FOnChangeCharacter OnChangeCharacter;
	
protected:
	virtual void BeginPlay() override;
	
private:
	
	UPROPERTY(VisibleAnywhere)
	USceneComponent* RootSceneComponent;
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* SkeletalMeshComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = "CharacterModelData")
	TMap<FGameplayTag, FCharacterModelData> CharacterModelDataArray;
	
};
