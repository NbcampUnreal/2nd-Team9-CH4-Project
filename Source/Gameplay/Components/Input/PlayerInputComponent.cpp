PRAGMA_DISABLE_OPTIMIZATION

#include "PlayerInputComponent.h"
#include "Gameplay/Defines/InputBuffer/InputBufferEntry.h"
#include "Gameplay/PlayerController/SSBPlayerController.h"
#include "Gameplay/Defines/Command/CommandRow.h"
#include "GameCore/Fighter/Fighter.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameCore/Ability/AbilityManager/AbilityManager.h"
#include "InputActionValue.h"
#include "Gameplay/Defines/InputBuffer/InputBufferEntry.h"
#include "Subsystems/GameInstanceSubsystem.h"

UPlayerInputComponent::UPlayerInputComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
}

void UPlayerInputComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (const ASSBPlayerController* PC = Cast<ASSBPlayerController>(GetOwner()))
	{
		if (PC->IsLocalController())
		{
			Player = PC->GetPawn<AFighter>(); 
			AddMappingContext(PC);
			BindActions(PC);	
		}
	}

	if (CommandTable[static_cast<int32>(ECharacterType::Fighter)].ToSoftObjectPath().IsValid())
	{
		if (const UDataTable* Table = CommandTable[0].LoadSynchronous())
		{
			const FString ContextString(TEXT("Command Table AttackInput"));
			Table->GetAllRows<FCommandRow>(ContextString, CommandRows);
		}
	}
	if (CommandTable[static_cast<int32>(ECharacterType::Anubis)].ToSoftObjectPath().IsValid())
	{
		if (const UDataTable* Table = CommandTable[1].LoadSynchronous())
		{
			const FString ContextString(TEXT("Command Table MoveInput"));
			Table->GetAllRows<FCommandRow>(ContextString, AnubisCommandRows);
		}
	}
	if (CommandTable[static_cast<int32>(ECharacterType::Gunner)].ToSoftObjectPath().IsValid())
	{
		if (const UDataTable* Table = CommandTable[2].LoadSynchronous())
		{
			const FString ContextString(TEXT("Command Table MoveInput"));
			Table->GetAllRows<FCommandRow>(ContextString, GunnerCommandRows);
		}
	}
}

void UPlayerInputComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	float CurrentTime = GetWorld()->GetTimeSeconds();
	
	for (int32 i = MoveInputBuffer.Num() - 1; i >= 0; --i)
	{
		if (CurrentTime - MoveInputBuffer[i].InputTime > 0.5f)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Remove Input Tag: %s \n Input Time: %f \n Current Time: %f"), *MoveInputBuffer[i].InputTag.ToString(), MoveInputBuffer[i].InputTime, CurrentTime);
			MoveInputBuffer.RemoveAt(i);
		}
	}
}

void UPlayerInputComponent::MoveInput(const FInputActionValue& InputValue)
{
	const FGameplayTag CurrentTag = GetInputTagFromValue(InputValue);
	const float CurrentTime = GetWorld()->GetTimeSeconds();

	const FInputBufferEntry NewEntry(CurrentTag, CurrentTime);
	if (MoveInputBuffer.Num() > 0)
	{
		FInputBufferEntry& LastEntry = MoveInputBuffer.Last();
		if (LastEntry.InputTag == CurrentTag)
		{
			if (CurrentTime - LastEntry.InputTime >= 0.05f)
			{
				MoveInputBuffer.Add(NewEntry);
				UE_LOG(LogTemp, Warning, TEXT("Add New Input Tag: %s \n Input Time: %f"), *CurrentTag.ToString(), CurrentTime);
			}
			else
			{
				LastEntry.InputTime = CurrentTime;
				//UE_LOG(LogTemp, Warning, TEXT("Update Input Tag: %s \n Updated Input Time: %f"), *CurrentTag.ToString(), CurrentTime);
			}
		}
		else
		{
			MoveInputBuffer.Add(NewEntry);
			UE_LOG(LogTemp, Warning, TEXT("Add Different Input Tag: %s \n Input Time: %f"), *CurrentTag.ToString(), CurrentTime);
		}
	}
	else
	{
		MoveInputBuffer.Add(NewEntry);
	}

	Player->Move(InputValue);
}


void UPlayerInputComponent::AttackInput(const FInputActionValue& InputValue, const FGameplayTag& AttackTag)
{
	/* NotifyState : Returns if buffering is true */
	if (GetWorld()->GetGameInstance()->GetSubsystem<UAbilityManager>()->CheckCurrentPlayingMontage()
		&& !Player.Get()->GetBuffering())
	{
		return;
	}

	/* Check Buffering */
	bool bIsAttack = false;
	if (GetWorld()->GetGameInstance()->GetSubsystem<UAbilityManager>()->CheckCurrentPlayingMontage())
	{
		bIsAttack = true;
	}

	TArray<FCommandRow*>* CurrentCommandRows = nullptr;
	ECharacterType PlayerType = Player->GetPlayerType();
	if (PlayerType == ECharacterType::Fighter)
	{
		CurrentCommandRows = &CommandRows;
	}
	else if (PlayerType == ECharacterType::Anubis)
	{
		CurrentCommandRows = &AnubisCommandRows;
	}
	else if (PlayerType == ECharacterType::Gunner)
	{
		CurrentCommandRows = &GunnerCommandRows;
	}
	
	FCommandRow* MatchingRow = nullptr;
	for (FCommandRow* Row : *CurrentCommandRows)
	{
		if (!Row || Row->AttackTag != AttackTag)
		{
			continue;
		}

		const TArray<FGameplayTag>& CommandSequence = Row->InputSequence;
		int32 SeqLength = CommandSequence.Num();

		if (MoveInputBuffer.Num() < SeqLength)
		{
			continue;
		}
		
		TArray<FInputBufferEntry> InputSegment;
		for (int32 i = MoveInputBuffer.Num() - SeqLength; i < MoveInputBuffer.Num(); ++i)
		{
			InputSegment.Add(MoveInputBuffer[i]);
		}

		if (InputSegment.Num() > 0)
		{
			float TotalInputDuration = InputSegment.Last().InputTime - InputSegment[0].InputTime;
			if (TotalInputDuration > Row->InputTime)
			{
				continue;
			}	
		}
		
		bool bMatch = true;
		for (int32 i = 0; i < SeqLength; ++i)
		{
			if (InputSegment[i].InputTag != CommandSequence[i])
			{
				bMatch = false;
				break;
			}
		}
		
		if (bMatch)
		{
			if (!MatchingRow || Row->Priority > MatchingRow->Priority)
			{
				MatchingRow = Row;
			}
		}
	}

	//MatchingCount
	if (MatchingRow)
	{
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle,	FTimerDelegate::CreateLambda([this]()
			{
				MoveInputBuffer.Empty();
			}),	0.1f,false);
		UE_LOG(LogTemp, Warning, TEXT("Command Matching!!: %s"), *MatchingRow->CommandName.ToString());
		GetWorld()->GetGameInstance()->GetSubsystem<UAbilityManager>()->RequestCreateAbility(MatchingRow->CommandName, bIsAttack);
		if (!bIsAttack)
		{
			Player->LockTag();	
		}
		else
		{
			Player->CurrentMontageName = GetWorld()->GetGameInstance()->GetSubsystem<UAbilityManager>()->GetNextMontageName();
			Player->RefreshlockTag();
		}
		return;
	}
    
	UE_LOG(LogTemp, Warning, TEXT("No matching, AttackTag: %s"), *AttackTag.ToString());
}

FGameplayTag UPlayerInputComponent::GetInputTagFromValue(const FInputActionValue& InputValue)
{
	FVector2D Direction = InputValue.Get<FVector2D>();

	FString XTag;
	if (Direction.X < 0)
	{
		if (!Player.Get()->GetPlayerLookingRight())
		{
			XTag = TEXT("Right");
		}
		else
		{
			XTag = TEXT("Left");			
		}
	}
	else if (Direction.X > 0)
	{
		if (!Player.Get()->GetPlayerLookingRight())
		{
			XTag = TEXT("Left");
		}
		else
		{
			XTag = TEXT("Right");
		}
	}

	FString YTag;
	if (Direction.Y < 0)
	{
		YTag = TEXT("Down");
	}
	else if (Direction.Y > 0)
	{
		YTag = TEXT("Up");
	}
	
	if (!XTag.IsEmpty() && !YTag.IsEmpty())
	{
		if (MoveInputBuffer.Num() > 0)
		{
			FGameplayTag CardinalX = FGameplayTag::RequestGameplayTag(FName(*FString::Printf(TEXT("Input.Move.%s"), *XTag)));
			FGameplayTag CardinalY = FGameplayTag::RequestGameplayTag(FName(*FString::Printf(TEXT("Input.Move.%s"), *YTag)));
			const FGameplayTag& LastTag = MoveInputBuffer.Last().InputTag;
			
			if (LastTag == CardinalY)
			{
				return CardinalX;
			}
			else if (LastTag == CardinalX)
			{
				if (MoveInputBuffer.Num() > 1)
				{
					const FGameplayTag& OneFromLastTag = MoveInputBuffer.Last(1).InputTag;
					if (OneFromLastTag == CardinalY)
					{
						return CardinalX;
					}
					else
					{
						return CardinalY;
					}
				}
				else
				{
					return CardinalY;
				}
			}
			else
			{
				return CardinalY;
			}
		}
		else
		{
			return FGameplayTag::RequestGameplayTag(FName(*FString::Printf(TEXT("Input.Move.%s"), *XTag)));
		}
	}
	else if (!XTag.IsEmpty())
	{
		return FGameplayTag::RequestGameplayTag(FName(*FString::Printf(TEXT("Input.Move.%s"), *XTag)));
	}
	else if (!YTag.IsEmpty())
	{
		return FGameplayTag::RequestGameplayTag(FName(*FString::Printf(TEXT("Input.Move.%s"), *YTag)));
	}
    
	return FGameplayTag::RequestGameplayTag(FName(TEXT("Input.Move.None")));
}

void UPlayerInputComponent::AddMappingContext(const ASSBPlayerController* PlayerController) const
{
	if (const ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMappingContext)
			{
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}
}

void UPlayerInputComponent::BindActions(const ASSBPlayerController* PlayerController)
{
	if (Player.IsValid())
	{
		if (UEnhancedInputComponent* InputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			InputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &UPlayerInputComponent::MoveInput);
			InputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, Player.Get(), &AFighter::Move);
			InputComponent->BindAction(JumpAction, ETriggerEvent::Started, Player.Get(), &AFighter::StartJump);
			InputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, Player.Get(), &AFighter::SetChangeStandTag);
			InputComponent->BindAction(ChangeLookAction, ETriggerEvent::Completed, Player.Get(), &AFighter::ChangeLook);
			InputComponent->BindAction(BlockAction, ETriggerEvent::Started, Player.Get(), &AFighter::StartBlocking);
			InputComponent->BindAction(BlockAction, ETriggerEvent::Completed, Player.Get(), &AFighter::EndBlocking);
			
			InputComponent->BindAction(WeakAttackAction, ETriggerEvent::Started, this, &UPlayerInputComponent::OnWeakAttack);
			InputComponent->BindAction(HeavyAttackAction, ETriggerEvent::Started, this, &UPlayerInputComponent::OnHeavyAttack);
			InputComponent->BindAction(SmashAttackAction, ETriggerEvent::Started, this, &UPlayerInputComponent::OnSmashAttack);
			InputComponent->BindAction(GrabAttackAction, ETriggerEvent::Started, this, &UPlayerInputComponent::OnGrabAttack);
		}
	}
}

void UPlayerInputComponent::OnWeakAttack(const FInputActionValue& InputValue)
{
	AttackInput(InputValue, FGameplayTag::RequestGameplayTag(FName(TEXT("Input.Attack.Weak"))));
}

void UPlayerInputComponent::OnHeavyAttack(const FInputActionValue& InputValue)
{
	AttackInput(InputValue, FGameplayTag::RequestGameplayTag(FName(TEXT("Input.Attack.Heavy"))));
}

void UPlayerInputComponent::OnSmashAttack(const FInputActionValue& InputValue)
{
	AttackInput(InputValue, FGameplayTag::RequestGameplayTag(FName(TEXT("Input.Attack.Smash"))));
}

void UPlayerInputComponent::OnGrabAttack(const FInputActionValue& InputValue)
{
	AttackInput(InputValue, FGameplayTag::RequestGameplayTag(FName(TEXT("Input.Attack.Grab"))));
}

