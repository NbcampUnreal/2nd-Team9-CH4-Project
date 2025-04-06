﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
//Test
#include "GameplayTagContainer.h"
#include "Fighter.generated.h"

class UCameraComponent;
class USpringArmComponent;
struct FInputActionValue;

//Test
class UAbilityManager;


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
	void IWantToSleep() const;
	
	UFUNCTION()
	void ImSleepy(const FString& MessageType, UObject* Payload);
	
	UFUNCTION()
	void Move(const FInputActionValue& InputValue);

	UFUNCTION()
	virtual void StartJump(const FInputActionValue& InputValue);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;


	//Test
	public:
	FGameplayTagContainer AbilityTagContainer;
	FGameplayTagContainer& GetCurrentTags() { return AbilityTagContainer; }
};
