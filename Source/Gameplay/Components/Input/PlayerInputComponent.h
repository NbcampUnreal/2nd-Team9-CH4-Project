﻿#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "PlayerInputComponent.generated.h"

class AFighter;
class ASSBPlayerController;
class UInputMappingContext;
class UInputAction;
struct FCommandRow;
struct FInputActionValue;
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

public:
	AFighter* GetFighter() const { return Player; }
	void SetFighter(AFighter* InFighter);
	
	void AddMappingContext(const ASSBPlayerController* PlayerController) const;
	void BindActions(const ASSBPlayerController* PlayerController);
	void FindFighter();

protected: /* Command Input */
	void MoveInput(const FInputActionValue& InputValue);
	void AttackInput(const FInputActionValue& InputValue, const FGameplayTag& AttackTag);
	void MatchCommand();
	FGameplayTag GetInputTagFromValue(const FInputActionValue& InputValue);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input")
	TArray<FInputBufferEntry>	MoveInputBuffer;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Command")
	TArray<TSoftObjectPtr<UDataTable>>	CommandTable;
	TArray<FCommandRow*>		CommandRows;
	TArray<FCommandRow*>		AnubisCommandRows;
	TArray<FCommandRow*>		GunnerCommandRows;
	AFighter* Player;
private: /* Key Input */
	
	

	void OnWeakAttack(const FInputActionValue& InputValue);
	void OnHeavyAttack(const FInputActionValue& InputValue);
	void OnSmashAttack(const FInputActionValue& InputValue);
	void OnGrabAttack(const FInputActionValue& InputValue);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* InputMappingContext{ nullptr };
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* CrouchAction{ nullptr };
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* WeakAttackAction{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* HeavyAttackAction{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* SmashAttackAction{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* GrabAttackAction{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* ChangeLookAction{ nullptr };
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* BlockAction{ nullptr };
};


