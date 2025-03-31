// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Fighter.generated.h"

struct FInputActionValue;

UCLASS()
class GAMECORE_API AFighter : public ACharacter
{
	GENERATED_BODY()

public:
	AFighter();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
public:
	UFUNCTION()
	void Move(const FInputActionValue& InputValue);

	UFUNCTION()
	virtual void StartJump(const FInputActionValue& InputValue);
};
