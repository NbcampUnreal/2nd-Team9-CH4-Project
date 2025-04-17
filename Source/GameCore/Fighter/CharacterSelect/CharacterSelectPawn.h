#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CharacterSelectPawn.generated.h"

class UWidgetComponent;
struct FCharacterModelData;

UCLASS()
class GAMECORE_API ACharacterSelectPawn : public APawn
{
	GENERATED_BODY()

public:
	ACharacterSelectPawn();
	
	void InitLocation(const int32 OffsetY);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastChangeCharacterModel(const FCharacterModelData& CharacterModelData) const;
	
private:
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USceneComponent* RootSceneComponent;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USkeletalMeshComponent* SkeletalMeshComponent;
	
};
