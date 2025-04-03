﻿#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"
#include "CommandRow.generated.h"

/**
 *  Command Input Data
 */
USTRUCT(BlueprintType)
struct FCommandRow : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Command")
	FGameplayTag  CommandName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Command")
	TArray<FGameplayTag> InputSequence;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Command")
	float InputTime;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Command")
	int32 Priority;
};
