#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CharacterSelectPlayerController.generated.h"

class ACharacterSelectPawn;
struct FGameplayTag;

UCLASS(Abstract, Blueprintable)
class GAMEPLAY_API ACharacterSelectPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION(Server, Reliable)
	void ServerChangeCharacter(FName CharacterTypeTagName);

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

private:
	TObjectPtr<ACharacterSelectPawn> OwnerPlayerPawn;
};
