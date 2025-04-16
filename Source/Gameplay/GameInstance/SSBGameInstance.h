#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/GameInstance.h"
#include "SSBGameInstance.generated.h"

class UEOSLocalPlayerSubsystem;
class UEOSSessionSubsystem;

UCLASS()
class GAMEPLAY_API USSBGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	USSBGameInstance();

protected:
	virtual void Init() override;
	virtual void Shutdown() override;

	virtual void StartGameInstance() override;

public:
	UFUNCTION(BlueprintCallable)
	void ChangeLevel(const FName& LevelName);
	UFUNCTION(BlueprintCallable)
	FGameplayTag GetLevelTag() const { return CurrentLevelTag; }
	
	UFUNCTION(BlueprintCallable, Category="EOS Session")
	void CreateEOSSession(FName KeyName, const FString& KeyValue, const FString& MapName);
	
	UFUNCTION(BlueprintCallable, Category="EOS Session")
	void FindEOSSessions(FName SearchKey, const FString& SearchValue);
	
	UFUNCTION(BlueprintCallable, Category="EOS Session")
	void JoinEOSSession();

	FORCEINLINE int32 GetPlayerIndex() const { return PlayerIndex; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetPlayerIndex(const int32 Index) { PlayerIndex = Index; }
	FORCEINLINE int32 GetCharacterTypeIndex() const { return CharacterTypeIndex; }
	FORCEINLINE void SetCharacterTypeIndex(const int32 NewCharacterTypeIndex) { CharacterTypeIndex = NewCharacterTypeIndex; }

private:
	FGameplayTag CurrentLevelTag;

	int32 PlayerIndex;
	int32 CharacterTypeIndex;

	TObjectPtr<UEOSSessionSubsystem> EOSSessionSubsystem;
};
