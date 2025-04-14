#include "CharacterSelectGameMode.h"

#include "Gameplay/PlayerController/CharacterSelect/CharacterSelectPlayerController.h"
#include "Kismet/KismetSystemLibrary.h"

void ACharacterSelectGameMode::PostLogin(APlayerController* NewPlayer)
{
	ACharacterSelectPlayerController* PlayerController = Cast<ACharacterSelectPlayerController>(NewPlayer);
	if (IsValid(PlayerController))
	{
		UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Post Login : %d"), NextPlayerIndex));
		PlayerController->SetPlayerIndex(NextPlayerIndex++);
	}
	
	Super::PostLogin(NewPlayer);
}
