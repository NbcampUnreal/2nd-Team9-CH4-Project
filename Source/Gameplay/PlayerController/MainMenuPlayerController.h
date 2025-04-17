#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainMenuPlayerController.generated.h"

class UEOSLocalPlayerSubsystem;

UCLASS()
class GAMEPLAY_API AMainMenuPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	UEOSLocalPlayerSubsystem* GetEOSLocalPlayerSubsystem() const;
};
