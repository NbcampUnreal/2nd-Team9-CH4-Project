// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SSBPlayerController.generated.h"

class UPlayerInputComponent;
/**
 * 
 */
UCLASS()
class GAMEPLAY_API ASSBPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ASSBPlayerController();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UPlayerInputComponent* PlayerInputComponent;
};
