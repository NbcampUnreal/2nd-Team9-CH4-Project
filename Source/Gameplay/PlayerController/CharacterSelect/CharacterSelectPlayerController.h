#pragma once

#include "CoreMinimal.h"
#include "GameCore/CharacterModelData/CharacterModelDataAsset.h"
#include "GameFramework/PlayerController.h"

#include "CharacterSelectPlayerController.generated.h"

class ICharacterSelectHUDInterface;
class ACharacterSelectPawn;

UCLASS(Abstract, Blueprintable)
class GAMEPLAY_API ACharacterSelectPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ACharacterSelectPlayerController();

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UFUNCTION(Server, Reliable)
	void ServerChangeCharacter(bool bIsNext);

	UFUNCTION(Server, Reliable)
	void ServerUpdateReady(bool bIsReady);

	UFUNCTION(Server, Reliable)
	void ServerStartGame();

	UFUNCTION(Client, Reliable)
	void ClientUpdatePlayerReady(int32 TargetPlayerIndex, bool bIsReady);

	UFUNCTION(Client, Reliable)
	void ClientChangedCharacter();
	
	void ChangeCharacter(bool bIsNext);
	void UpdateButtonIsEnabled(bool bIsAllPlayersReady) const;
	
	FORCEINLINE int32 GetPlayerIndex() const { return PlayerIndex; }
	FORCEINLINE int32 GetSelectedCharacterTypeIndex() const { return SelectedCharacterTypeIndex; }
	
	void SetPlayerIndex(const int32 NewPlayerIndex);

private:
	UFUNCTION()
	void OnRep_PlayerIndex();
	
	void SpawnMainCamera();
	void UpdateViewTarget();
	void InitPawnLocation() const;
	
	UPROPERTY(Replicated)
	TObjectPtr<ACharacterSelectPawn> OwnerPlayerPawn;

	UPROPERTY()
	TObjectPtr<AActor> MainCamera;

	UPROPERTY(EditDefaultsOnly, Category = "CharacterSelectCamera")
	FVector MainCameraSpawnLocation;

	UPROPERTY(EditDefaultsOnly, Category = "CharacterModelData")
	TObjectPtr<UCharacterModelDataAsset> CharacterModelDataAsset;

	UPROPERTY(ReplicatedUsing = OnRep_PlayerIndex)
	int32 PlayerIndex;
	int32 SelectedCharacterTypeIndex;
	int32 MaxCharacterTypeIndex;
	
	ICharacterSelectHUDInterface* CharacterSelectHUDInterface;
};
