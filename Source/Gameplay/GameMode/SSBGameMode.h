﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "SSBGameMode.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPLAY_API ASSBGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	virtual void PostLogin(APlayerController* NewPlayer) override;

	ASSBGameMode();
};
