// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "GameCore/Interface/PlayerStateInterface.h"
#include "SSBPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPLAY_API ASSBPlayerState : public APlayerState, public IPlayerStateInterface
{
	GENERATED_BODY()

public:
	ASSBPlayerState();

	virtual void BeginPlay() override;
	
public: /* IPlayerStateInterface */
	virtual int32 GetPlayerScore() const override { return PlayerScore; }
	virtual void SetPlayerScore(int32 NewScore) override { PlayerScore = NewScore; }

private:
	int32 PlayerScore;
};
