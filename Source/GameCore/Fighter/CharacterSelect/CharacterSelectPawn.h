#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CharacterSelectPawn.generated.h"

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
	
protected:
	virtual void BeginPlay() override;
	
private:
	
	UPROPERTY(VisibleAnywhere)
	USceneComponent* RootSceneComponent;
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* SkeletalMeshComponent;
};
