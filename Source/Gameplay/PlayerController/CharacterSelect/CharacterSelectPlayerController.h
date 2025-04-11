#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CharacterSelectPlayerController.generated.h"

class UCameraComponent;
class ACharacterSelectPawn;
struct FGameplayTag;

UCLASS(Abstract, Blueprintable)
class GAMEPLAY_API ACharacterSelectPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ACharacterSelectPlayerController();
	
	UFUNCTION(Server, Reliable)
	void ServerChangeCharacter(FName CharacterTypeTagName);

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

private:
	void SpawnMainCamera();
	void UpdateViewTarget();
	
	UPROPERTY()
	TObjectPtr<ACharacterSelectPawn> OwnerPlayerPawn;

	UPROPERTY()
	TObjectPtr<AActor> MainCamera;
	
	UPROPERTY(EditDefaultsOnly, Category = "CharacterSelectCamera")
	FVector MainCameraSpawnLocation;
};
