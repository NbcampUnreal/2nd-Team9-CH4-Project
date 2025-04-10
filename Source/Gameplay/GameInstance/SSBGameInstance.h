#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/GameInstance.h"
#include "SSBGameInstance.generated.h"

UCLASS()
class GAMEPLAY_API USSBGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	USSBGameInstance();

protected:
	virtual void Init() override;
	virtual void Shutdown() override;

public:
	UFUNCTION(BlueprintCallable)
	void ChangeLevel(const FName& LevelName);
	UFUNCTION(BlueprintCallable)
	FGameplayTag GetLevelTag() const { return CurrentLevelTag; }
private:
	FGameplayTag CurrentLevelTag;
};
