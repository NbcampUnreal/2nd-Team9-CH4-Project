#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerInputComponent.generated.h"

class ASSBPlayerController;
class UInputMappingContext;
class UInputAction;
struct FInputBufferEntry;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMEPLAY_API UPlayerInputComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPlayerInputComponent();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;

private:
	void MoveInput(const FInputActionValue& InputValue);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TArray<FInputBufferEntry> MoveInputBuffer;
	
private:
	void AddMappingContext(const ASSBPlayerController* PlayerController) const;
	void BindActions(const ASSBPlayerController* PlayerController);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* InputMappingContext{ nullptr };
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction{ nullptr };
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction{ nullptr };
};
