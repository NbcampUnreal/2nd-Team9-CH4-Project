#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SSBCharacterSelectPlayerController.generated.h"

struct FInputActionValue;
class UInputAction;
class UInputMappingContext;

UCLASS(Abstract, Blueprintable)
class GAMEPLAY_API ASSBCharacterSelectPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ASSBCharacterSelectPlayerController();

protected:
	virtual void BeginPlay() override;
	
private:
	void AddMappingContext() const;
	void BindInputActions();
	
	void SelectorMove(const FInputActionValue& InputValue);
	void Select(const FInputActionValue& InputValue);
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* SelectorMoveAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* SelectAction;
};
