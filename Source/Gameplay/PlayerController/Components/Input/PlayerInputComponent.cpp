#include "PlayerInputComponent.h"
#include "../../SSBPlayerController.h"
#include "Gameplay/Defines/Command/CommandRow.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameCore/Fighter/Fighter.h"
#include "InputActionValue.h"
#include "Gameplay/Defines/InputBuffer/InputBufferEntry.h"

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
			AddMappingContext(PC);
			BindActions(PC);	
		}
	}

	if (CommandTable)
	{
		if (UDataTable* CommandDataTable = CommandTable.LoadSynchronous())
		{
			const FString ContextString(TEXT("Command Table AttackInput"));
			CommandDataTable->GetAllRows<FCommandRow>(ContextString, CommandRows);	
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
		if (CurrentTime - MoveInputBuffer[i].InputTime > 1.0f)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Remove Input Tag: %s \n Input Time: %f \n Current Time: %f"), *MoveInputBuffer[i].InputTag.ToString(), MoveInputBuffer[i].InputTime, CurrentTime);
			MoveInputBuffer.RemoveAt(i);
		}
	}
}

void UPlayerInputComponent::MoveInput(const FInputActionValue& InputValue)
{
	FInputBufferEntry BufferEntry;
	BufferEntry.InputTag = GetInputTagFromValue(InputValue);
	BufferEntry.InputTime = GetWorld()->GetTimeSeconds();

	if (MoveInputBuffer.Num() > 0 && MoveInputBuffer.Last().InputTag == BufferEntry.InputTag)
	{
		MoveInputBuffer.Last().InputTime = BufferEntry.InputTime;
		//UE_LOG(LogTemp, Warning, TEXT("Correction Input Tag: %s \n Input Time: %f"), *BufferEntry.InputTag.ToString(), BufferEntry.InputTime);
	}
	else
	{
		MoveInputBuffer.Add(BufferEntry);
		UE_LOG(LogTemp, Warning, TEXT("Add Input Tag: %s \n Input Time: %f"), *BufferEntry.InputTag.ToString(), BufferEntry.InputTime);
	}
}

void UPlayerInputComponent::AttackInput(const FInputActionValue& InputValue, const FGameplayTag& AttackTag)
{
	for (FCommandRow* Row : CommandRows)
	{
		if (!Row || Row->AttackTag != AttackTag)
		{
			continue;
		}
        
		/*
		 * 이거 고민해봐야 할듯 각 Row의 Sequence의 길이만큼 짤라서 쓸지
		 * for문 밖에서 역순으로 할지
		 */
		const TArray<FGameplayTag>& CommandSequence = Row->InputSequence;
		int32 SeqLength = CommandSequence.Num();
		if (MoveInputBuffer.Num() < SeqLength)
		{
			continue;
		}

		TArray<FGameplayTag> InputSegment;
		for (int32 i = MoveInputBuffer.Num() - SeqLength; i < MoveInputBuffer.Num(); ++i)
		{
			InputSegment.Add(MoveInputBuffer[i].InputTag);
		}
		
		bool bMatch = true;
		for (int32 i = 0; i < SeqLength; ++i)
		{
			if (InputSegment[i] != CommandSequence[i])
			{
				bMatch = false;
				break;
			}
		}
 
		if (bMatch)
		{
			UE_LOG(LogTemp, Warning, TEXT("Command Matching!!: %s"), *Row->CommandName.ToString());
			//ExecuteCommand(Row->CommandName);
			return;
		}
	}
    
	UE_LOG(LogTemp, Warning, TEXT("No matching, AttackTag: %s"), *AttackTag.ToString());
}


FGameplayTag UPlayerInputComponent::GetInputTagFromValue(const FInputActionValue& InputValue)
{
	FVector2D Direction = InputValue.Get<FVector2D>();

	FString XTag;
	if (Direction.X < 0)
	{
		XTag = TEXT("Left");
	}
	else if (Direction.X > 0)
	{
		XTag = TEXT("Right");
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
		return FGameplayTag::RequestGameplayTag(FName(*FString::Printf(TEXT("Input.Move.%s%s"), *XTag, *YTag)));
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
	if (AFighter* Fighter = Cast<AFighter>(PlayerController->GetPawn()))
	{
		if (UEnhancedInputComponent* InputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			InputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &UPlayerInputComponent::MoveInput);
			
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

