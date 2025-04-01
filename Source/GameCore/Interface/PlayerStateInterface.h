// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerStateInterface.generated.h"

UINTERFACE()
class UPlayerStateInterface : public UInterface
{
	GENERATED_BODY()
};

class GAMECORE_API IPlayerStateInterface
{
	GENERATED_BODY()

public:
	virtual int32 GetPlayerScore() const = 0;
	virtual void SetPlayerScore(int32 NewScore) = 0;
};
