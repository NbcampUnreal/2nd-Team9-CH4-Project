#pragma once

#include "CoreMinimal.h"
#include "GameCore/CharacterModelData/CharacterModelDataAsset.h"
#include "GameFramework/PlayerController.h"

#include "CharacterSelectPlayerController.generated.h"

class ACharacterSelectPawn;

UCLASS(Abstract, Blueprintable)
class GAMEPLAY_API ACharacterSelectPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ACharacterSelectPlayerController();

	UFUNCTION(Server, Reliable)
	void ServerChangeCharacter(int32 TargetPlayerIndex, int32 CharacterTypeIndex);

	UFUNCTION(Client, Reliable)
	void ClientUpdateCharacterIconTexture(int32 TargetPlayerIndex, UTexture2D* IconTexture);

	UFUNCTION(Client, Reliable)
	void ClientUpdatePlayerReady(int32 TargetPlayerIndex, bool bIsReady, bool bIsAllReady);


	FORCEINLINE int32 GetNumCharacterModelData() const
	{
		if (IsValid(CharacterModelDataAsset))
		{
			return CharacterModelDataAsset->GetNumCharacterModelData();
		}

		return 0;
	}

	FORCEINLINE int32 GetPlayerIndex() const { return PlayerIndex; }
	FORCEINLINE int32 GetSelectedCharacterTypeIndex() const { return SelectedCharacterTypeIndex; }
	
	FORCEINLINE void SetPlayerIndex(const int32 NewPlayerIndex) { PlayerIndex = NewPlayerIndex; }

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

private:
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
	
	int32 PlayerIndex;
	int32 SelectedCharacterTypeIndex;
};
