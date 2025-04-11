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

	FORCEINLINE int32 GetPlayerIndex() const { return PlayerIndex; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetPlayerIndex(const int32 Index) { PlayerIndex = Index; }
	FORCEINLINE FGameplayTag GetCharacterTypeTag() const { return CharacterTypeTag; }
	FORCEINLINE void SetCharacterTypeTag(const FGameplayTag NewCharacterTypeTag) { CharacterTypeTag = NewCharacterTypeTag; }
private:
	FGameplayTag CurrentLevelTag;

	int32 PlayerIndex;
	FGameplayTag CharacterTypeTag;
};
