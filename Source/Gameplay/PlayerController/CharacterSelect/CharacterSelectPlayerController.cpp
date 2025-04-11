#include "CharacterSelectPlayerController.h"

#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "GameCore/Fighter/CharacterSelect/CharacterSelectPawn.h"

ACharacterSelectPlayerController::ACharacterSelectPlayerController()
{
	MainCameraSpawnLocation = FVector::ZeroVector;
	bAutoManageActiveCameraTarget = false;
}

void ACharacterSelectPlayerController::ServerChangeCharacter_Implementation(const FName CharacterTypeTagName)
{
	if (IsValid(OwnerPlayerPawn))
	{
		OwnerPlayerPawn->MulticastChangeCharacterModel(CharacterTypeTagName);
	}
}

void ACharacterSelectPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetShowMouseCursor(true);
	SpawnMainCamera();
}

void ACharacterSelectPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	OwnerPlayerPawn = Cast<ACharacterSelectPawn>(InPawn);
	SpawnMainCamera();
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
