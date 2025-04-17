// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSelectPlayerState.h"

#include "Gameplay/PlayerController/CharacterSelect/CharacterSelectPlayerController.h"
#include "Net/UnrealNetwork.h"

void ACharacterSelectPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACharacterSelectPlayerState, bIsReady);
	DOREPLIFETIME(ACharacterSelectPlayerState, OwnerPlayerController);
}

void ACharacterSelectPlayerState::OnRep_bIsReady() const
{
	ACharacterSelectPlayerController* LocalController
		= Cast<ACharacterSelectPlayerController>(GetWorld()->GetFirstPlayerController());
	if (IsValid(OwnerPlayerController) && IsValid(LocalController))
	{
		LocalController->UpdateReady(OwnerPlayerController->GetPlayerIndex(), bIsReady);
	}
}
