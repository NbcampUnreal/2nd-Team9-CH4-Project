#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "CharacterModelDataAsset.generated.h"

USTRUCT(BlueprintType)
struct FCharacterModelData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag CharacterTypeTag;
	UPROPERTY(EditDefaultsOnly)
	USkeletalMesh* SkeletalMesh;
	UPROPERTY(EditDefaultsOnly)
	TArray<UMaterialInterface*> MaterialArray;
	UPROPERTY(EditDefaultsOnly)
	UAnimationAsset* IdleAnimation;
	UPROPERTY(EditDefaultsOnly)
	UTexture2D* IconTexture;
};

UCLASS(Blueprintable, BlueprintType)
class GAMECORE_API UCharacterModelDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	FORCEINLINE FCharacterModelData GetCharacterModelDataByIndex(const int32 Index) const
	{
		if (CharacterModelDataArray.IsValidIndex(Index))
		{
			return CharacterModelDataArray[Index];
		}
		return FCharacterModelData();
	}

	FORCEINLINE int32 GetNumCharacterModelData() const { return CharacterModelDataArray.Num(); }

private:
	UPROPERTY(EditDefaultsOnly)
	TArray<FCharacterModelData> CharacterModelDataArray;
};
