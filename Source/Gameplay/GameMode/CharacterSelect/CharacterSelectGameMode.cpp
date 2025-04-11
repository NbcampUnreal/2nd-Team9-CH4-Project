#include "CharacterSelectGameMode.h"

#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"

AActor* ACharacterSelectGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	TArray<AActor*> PlayerStarts;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStarts);

	if (PlayerStarts.Num() > 0)
	{
		AActor* PlayerStart = PlayerStarts[CurrentSpawnIndex];
		CurrentSpawnIndex++;
		return PlayerStart;
	}
	
	return Super::ChoosePlayerStart_Implementation(Player);
}
