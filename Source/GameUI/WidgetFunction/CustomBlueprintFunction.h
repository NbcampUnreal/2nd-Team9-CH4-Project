#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CustomBlueprintFunction.generated.h"


UCLASS()
class GAMEUI_API UCustomBlueprintFunction : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Network")
	static FString GetLocalIPAddress();
};
