#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "CharacterSelectPlayerState.generated.h"

class ACharacterSelectPlayerController;

UCLASS()
class GAMEPLAY_API ACharacterSelectPlayerState : public APlayerState
{
	GENERATED_BODY()

protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

public:

	FORCEINLINE bool IsReady() const { return bIsReady; }
	FORCEINLINE void UpdateReady(const bool bNewReady)
	{
		bIsReady = bNewReady;
		OnRep_bIsReady();
	}
	
private:
	
	UFUNCTION()
	void OnRep_bIsReady() const;
	
	UPROPERTY(ReplicatedUsing=OnRep_bIsReady)
	bool bIsReady;

	UPROPERTY(Replicated)
	TObjectPtr<ACharacterSelectPlayerController> OwnerPlayerController;
};
