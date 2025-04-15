// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AbilityManagerHelper.generated.h"

enum class ECharacterType : uint8;

UCLASS(Blueprintable)
class GAMECORE_API UAbilityManagerHelper : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, blueprintReadWrite, Category = "Ability")
	TArray<TSoftObjectPtr<UDataTable>> AbilityTableArray;

	UPROPERTY(EditAnywhere, blueprintReadWrite, Category = "Anim")
	TArray<TSoftObjectPtr<UDataTable>> AnimTableArray;
	
	// UPROPERTY(EditAnywhere, blueprintReadWrite, Category = "Ability")
	// TSoftObjectPtr<UDataTable> AbilityDataTable;
	//
	// UPROPERTY(EditAnywhere, blueprintReadWrite, Category = "Anim")
	// TSoftObjectPtr<UDataTable> AnimDataTable;
};
