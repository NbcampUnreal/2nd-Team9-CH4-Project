#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "InputBufferEntry.generated.h"

USTRUCT(BlueprintType)
struct GAMEPLAY_API FInputBufferEntry
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    FGameplayTag InputTag;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    float InputTime;
    
    FInputBufferEntry() : InputTag(FGameplayTag()), InputTime(0.f) {}
    FInputBufferEntry(FGameplayTag InTag, float InTime) : InputTag(InTag), InputTime(InTime) {}
};


