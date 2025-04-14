#include "CharacterSelectPlayerController.h"

#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "GameCore/Fighter/CharacterSelect/CharacterSelectPawn.h"
#include "GameCore/Interface/CharacterSelect/CharacterSelectHUDInterface.h"
#include "GameFramework/HUD.h"
#include "Gameplay/GameInstance/SSBGameInstance.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"

ACharacterSelectPlayerController::ACharacterSelectPlayerController()
{
	MainCameraSpawnLocation = FVector::ZeroVector;
	bAutoManageActiveCameraTarget = false;
	PlayerIndex = 0;
	SelectedCharacterTypeIndex = 0;
}

void ACharacterSelectPlayerController::ServerChangeCharacter_Implementation(
	const int32 TargetPlayerIndex, const int32 CharacterTypeIndex)
{
	if (IsValid(OwnerPlayerPawn) && IsValid(CharacterModelDataAsset))
	{
		SelectedCharacterTypeIndex = CharacterTypeIndex;

		const FCharacterModelData CharacterModelData = CharacterModelDataAsset->GetCharacterModelDataByIndex(
			CharacterTypeIndex);
		OwnerPlayerPawn->MulticastChangeCharacterModel(CharacterModelData);

		int32 Index = 0;
		for (auto It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
		{
			ACharacterSelectPlayerController* CharacterSelectPlayerController
				= Cast<ACharacterSelectPlayerController>(*It);
			if (IsValid(CharacterSelectPlayerController))
			{
				CharacterSelectPlayerController->ClientUpdateCharacterIconTexture(TargetPlayerIndex,
					CharacterModelData.IconTexture);
			}

			Index++;
		}
	}
}

void ACharacterSelectPlayerController::ClientUpdateCharacterIconTexture_Implementation(const int32 TargetPlayerIndex,
	UTexture2D* IconTexture)
{
	AHUD* HUD = GetHUD();
	if (IsValid(HUD))
	{
		if (ICharacterSelectHUDInterface* CharacterSelectHUDInterface = Cast<ICharacterSelectHUDInterface>(HUD))
		{
			CharacterSelectHUDInterface->UpdateCharacterIconTexture(TargetPlayerIndex, IconTexture);
		}
	}
}

void ACharacterSelectPlayerController::ClientUpdatePlayerReady_Implementation(
	const int32 TargetPlayerIndex, const bool bIsReady, const bool bIsAllReady)
{
	AHUD* HUD = GetHUD();
	if (IsValid(HUD))
	{
		if (ICharacterSelectHUDInterface* CharacterSelectHUDInterface = Cast<ICharacterSelectHUDInterface>(HUD))
		{
			CharacterSelectHUDInterface->UpdatePlayerReady(TargetPlayerIndex, bIsReady, bIsAllReady);
		}
	}
}

void ACharacterSelectPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetShowMouseCursor(true);
	SetInputMode(FInputModeUIOnly());
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
