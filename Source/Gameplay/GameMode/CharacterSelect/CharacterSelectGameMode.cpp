#include "CharacterSelectGameMode.h"

#include "GameCore/Fighter/CharacterSelect/CharacterSelectPawn.h"

void ACharacterSelectGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	const int32 PlayerIndex = GetWorld()->GetNumPlayerControllers() - 1;
	
	if (IsValid(NewPlayer))
	{
		APawn* Pawn = NewPlayer->GetPawn();
		ACharacterSelectPawn* CharacterSelectPawn = Cast<ACharacterSelectPawn>(Pawn);
		if (IsValid(CharacterSelectPawn))
		{
			const float OffsetY = PlayerIndex * 100.0f;
			CharacterSelectPawn->AddActorWorldOffset(FVector(0.0f, OffsetY, 0.0f));
		}
	}
}
