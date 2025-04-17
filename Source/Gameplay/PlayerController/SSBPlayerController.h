#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SSBPlayerController.generated.h"

class UPlayerInputComponent;

UCLASS()
class GAMEPLAY_API ASSBPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ASSBPlayerController();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UPlayerInputComponent* PlayerInputComponent;

protected:
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;

	virtual void PreProcessInput(const float DeltaTime, const bool bGamePaused) override;

	virtual void SetupInputComponent() override;

public:
	UFUNCTION(Client, Reliable)
	void Client_OnPossess();

	
	UFUNCTION(Client, Reliable)
	void Client_OnRespawnedPawn();

	FTimerHandle RespawnPawnCheckTimer;

	// 타이머 콜백
	void TryBindRespawnedPawn();
};
