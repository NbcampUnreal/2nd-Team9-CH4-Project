#include "CharacterSelectPlayerController.h"

#include "GameCore/Fighter/CharacterSelect/CharacterSelectPawn.h"

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
}

void ACharacterSelectPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	OwnerPlayerPawn = Cast<ACharacterSelectPawn>(InPawn);
}
