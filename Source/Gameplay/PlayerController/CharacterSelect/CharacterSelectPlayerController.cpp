#include "CharacterSelectPlayerController.h"

#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "GameCore/Fighter/CharacterSelect/CharacterSelectPawn.h"
#include "GameCore/Interface/CharacterSelect/CharacterSelectHUDInterface.h"
#include "GameFramework/HUD.h"
#include "Gameplay/GameInstance/SSBGameInstance.h"
#include "Gameplay/GameMode/CharacterSelect/CharacterSelectGameMode.h"
#include "Gameplay/GameState/CharacterSelect/CharacterSelectGameState.h"
#include "Gameplay/PlayerState/CharacterSelectPlayerState.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"

ACharacterSelectPlayerController::ACharacterSelectPlayerController()
{
	MainCameraSpawnLocation = FVector::ZeroVector;
	bAutoManageActiveCameraTarget = false;
	PlayerIndex = 0;
	SelectedCharacterTypeIndex = 0;
	MaxCharacterTypeIndex = 0;

	CharacterSelectHUDInterface = nullptr;
}

void ACharacterSelectPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetShowMouseCursor(true);
	SetInputMode(FInputModeUIOnly());

	CharacterSelectHUDInterface = Cast<ICharacterSelectHUDInterface>(GetHUD());

	if (IsValid(CharacterModelDataAsset))
	{
		MaxCharacterTypeIndex = CharacterModelDataAsset->GetNumCharacterModelData();
	}

	SpawnMainCamera();
}

void ACharacterSelectPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	OwnerPlayerPawn = Cast<ACharacterSelectPawn>(InPawn);
	SpawnMainCamera();

	InitPawnLocation();
}

void ACharacterSelectPlayerController::GetLifetimeReplicatedProps(
	TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACharacterSelectPlayerController, OwnerPlayerPawn);
	DOREPLIFETIME(ACharacterSelectPlayerController, SelectedCharacterTypeIndex);
}

void ACharacterSelectPlayerController::ServerChangeCharacter_Implementation(const bool bIsNextButton)
{
	if (IsValid(OwnerPlayerPawn) && IsValid(CharacterModelDataAsset))
	{
		if (bIsNextButton)
		{
			SelectedCharacterTypeIndex++;
			SelectedCharacterTypeIndex %= MaxCharacterTypeIndex;
		}
		else
		{
			if (SelectedCharacterTypeIndex == 0)
			{
				SelectedCharacterTypeIndex = MaxCharacterTypeIndex - 1;
			}
			else
			{
				SelectedCharacterTypeIndex--;
			}
		}

		OnRep_SelectedCharacterTypeIndex();
		
		OwnerPlayerPawn->MulticastChangeCharacterModel(
			CharacterModelDataAsset->GetCharacterModelDataByIndex(SelectedCharacterTypeIndex));
	}
}

void ACharacterSelectPlayerController::ServerUpdateReady_Implementation(const bool bIsReady)
{
	ACharacterSelectPlayerState* CharacterSelectPlayerState = Cast<ACharacterSelectPlayerState>(PlayerState);
	if (IsValid(CharacterSelectPlayerState))
	{
		CharacterSelectPlayerState->UpdateReady(bIsReady);

		ACharacterSelectGameState* GameState = Cast<ACharacterSelectGameState>(GetWorld()->GetGameState());
		if (IsValid(GameState))
		{
			GameState->NotifyPlayerReadyChanged();
		}
	}
}

void ACharacterSelectPlayerController::ServerStartGame_Implementation()
{
	// Check Is Host
	if (PlayerIndex == 0)
	{
		ACharacterSelectGameMode* GameMode = Cast<ACharacterSelectGameMode>(GetWorld()->GetAuthGameMode());
		if (IsValid(GameMode))
		{
			GameMode->TryStartGame();
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Is Not Host"))
	}
}

void ACharacterSelectPlayerController::ClientUpdateCharacterIcon_Implementation(const int32 TargetPlayerIndex,
	const int32 TargetSelectedCharacterIndex)
{
	if (IsValid(Cast<UObject>(CharacterSelectHUDInterface)))
	{
		CharacterSelectHUDInterface->UpdateCharacterIconTexture(
			TargetPlayerIndex,
			CharacterModelDataAsset->GetIconTextureByIndex(TargetSelectedCharacterIndex)
		);
	}
}

void ACharacterSelectPlayerController::UpdateReady(const int32 Index, const bool bIsReady) const
{
	if (IsValid(Cast<UObject>(CharacterSelectHUDInterface)))
	{
		CharacterSelectHUDInterface->UpdateReady(Index, bIsReady);
	}
}

void ACharacterSelectPlayerController::SetPlayerIndex(const int32 NewPlayerIndex)
{
	PlayerIndex = NewPlayerIndex;

	OnRep_PlayerIndex();
}

void ACharacterSelectPlayerController::OnRep_PlayerIndex()
{
	CharacterSelectHUDInterface = Cast<ICharacterSelectHUDInterface>(GetHUD());
	if (CharacterSelectHUDInterface)
	{
		CharacterSelectHUDInterface->SetupHUD();
	}
}

void ACharacterSelectPlayerController::OnRep_SelectedCharacterTypeIndex() const
{
	USSBGameInstance* GameInstance = Cast<USSBGameInstance>(GetGameInstance());
	if (IsValid(GameInstance))
	{
		GameInstance->SetSelectedCharacterTypeIndex(SelectedCharacterTypeIndex);
	}

	ACharacterSelectGameState* GameState = Cast<ACharacterSelectGameState>(GetWorld()->GetGameState());
	if (IsValid(GameState))
	{
		GameState->ServerUpdateCharacterIcon(PlayerIndex, SelectedCharacterTypeIndex);
	}
}

void ACharacterSelectPlayerController::SpawnMainCamera()
{
	if (!IsValid(MainCamera))
	{
		MainCamera = GetWorld()->SpawnActor(ACameraActor::StaticClass());
		const ACameraActor* CameraActor = Cast<ACameraActor>(MainCamera);
		UCameraComponent* CameraComponent = CameraActor->GetCameraComponent();
		CameraComponent->SetProjectionMode(ECameraProjectionMode::Orthographic);
		CameraComponent->SetOrthoWidth(650.0f);
		CameraComponent->SetAutoPlaneShift(false);
	}

	UpdateViewTarget();
}

void ACharacterSelectPlayerController::UpdateViewTarget()
{
	if (IsValid(MainCamera))
	{
		MainCamera->SetActorLocation(MainCameraSpawnLocation);
		SetViewTarget(MainCamera);
	}
}

void ACharacterSelectPlayerController::InitPawnLocation() const
{
	if (!IsValid(OwnerPlayerPawn))
	{
		UE_LOG(LogTemp, Error, TEXT("OwnerPlayerPawn Is Not Valid"));
		return;
	}

	UKismetSystemLibrary::PrintString(GetWorld(), FString::FromInt(PlayerIndex));

	const float OffsetY = PlayerIndex * 150.0f;
	OwnerPlayerPawn->InitLocation(OffsetY);
}
