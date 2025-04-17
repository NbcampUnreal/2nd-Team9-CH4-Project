#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "CharacterSelectPlayerState.generated.h"

class ACharacterSelectPlayerController;

UCLASS()
class GAMEPLAY_API ACharacterSelectPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	void UpdateReady(const bool bNewReady);

	FORCEINLINE bool IsReady() const { return bIsReady; }
	
private:
	bool bIsReady;
};
