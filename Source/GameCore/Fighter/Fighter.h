// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayTagContainer.h"
#include "Fighter.generated.h"

enum class ECharacterType : uint8;
class UHitComponent;
class UNiagaraComponent;
class UNiagaraSystem;
class UCameraComponent;
class USpringArmComponent;
struct FInputActionValue;

//Test
class UAbilityManager;


UCLASS()
class GAMECORE_API AFighter : public ACharacter
{
	GENERATED_BODY()

public:
	AFighter();
public:
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayMontage(UAnimMontage* Montage, float PlayRate = 1.f, FName StartSection = NAME_None);

	UFUNCTION(Server, Reliable)
	void Server_RequestPlayMontage(UAnimMontage* Montage, float PlayRate = 1.f, FName StartSection = NAME_None);

	void PlayMontageOnAllClients(UAnimMontage* Montage, float PlayRate = 1.f, FName StartSection = NAME_None);
	UFUNCTION(Server, Unreliable)
	void Server_UpdatePlayerTag(const FGameplayTag& NewTag);
	UFUNCTION(Server, Unreliable)
	void Server_UpdatePlayerTagContainer(const FGameplayTagContainer& NewTagContainer);
	UFUNCTION(Server, Unreliable)
	void Server_UpdateStandTag(const FString& NewStandTag);
	UFUNCTION(Server, Unreliable)
	void Server_UpdateLockTag(const FGameplayTag& NewTag);
	UFUNCTION(Server, Unreliable)
	void Server_UpdateLookingRight(const bool bInLookRight);
	UFUNCTION(Server, Unreliable)
	void Server_UpdateTickCrouch(const bool bInCheckTickCrouch);
	void MoveStart(const FInputActionValue& InputActionValue);
	void MoveEnd();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Landed(const FHitResult& Hit) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
public:
	UFUNCTION(BlueprintCallable)
	bool GetPlayerLookingRight() const { return bLookingRight; }
	
	UFUNCTION()
	void ImSleepy(const FString& MessageType, UObject* Payload);
	UFUNCTION()
	void Move(const FInputActionValue& InputValue);
	UFUNCTION()
	void StartJump(const FInputActionValue& InputValue);
	UFUNCTION()
	void SetCheckTickCrouch();
	
	void StartBlocking(const FInputActionValue& InputActionValue);
	void SetBlocking(const bool bInBlocking) { bBlocking = bInBlocking; }
	void EndBlocking(const FInputActionValue& InputActionValue);
	
	bool GetBuffering();
	void SetBuffering(const bool bBuffering) { bOnBuffering = bBuffering; }

	void HitStop(const FVector& LaunchVector = {}, const FGameplayTag& InHitTag = AttackTag, const bool bIsAttacker = true);
	void AfterLaunchHitStop();
	void AfterHitStop();
	void StopMovementTimerSet(UCharacterMovementComponent* MovementCom, float Time);

	ECharacterType GetPlayerType() const { return Type; }
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	UNiagaraSystem* NiagaraEffect;
	TArray<UNiagaraComponent*> NiagaraComponents;
	
public:
	UFUNCTION()
	void OnRep_CurrentPlayerTag();
	UFUNCTION(BlueprintCallable)
	void UnlockedTag();
	UFUNCTION(BlueprintCallable)
	void SetGameplayTag(const FGameplayTag& GameplayTag);
	UFUNCTION(BlueprintCallable)
	void SetCurrentStandTag(const FString& InStandTag);
	UFUNCTION(BlueprintCallable)
	FGameplayTag GetGameplayTag() const { return CurrentPlayerTag; }
	UFUNCTION(BlueprintCallable)
	FGameplayTagContainer& GetCurrentTags();
	
	void LockTag();
	void ChangeLook();
	void SetIdleTag();;
	void AddAttackTag();
	void RefreshlockTag();
	void RemoveAttackTag();
	void SetChangeBaseTag();
	void SetCrouch();
	void SetUnCrouch();
	/* Define 에 옮겨놔야할듯 */
	static FGameplayTag AttackTag;
	static FGameplayTag BaseTag;
	static FGameplayTag JumpTag;
	static FGameplayTag IdleTag;
	static FGameplayTag LandTag;
	static FGameplayTag HitTag;
	static FGameplayTag CrouchTag;
	static FGameplayTag BlockingTag;
	static FGameplayTag WeakHitTag;
	static FGameplayTag HeavyHitTag;
	static FGameplayTag LaunchHitTag;
private:
	// 앉아있는지 서있는지 초기화 할때 쓰이는 변수
	UPROPERTY(ReplicatedUsing = OnRep_CurrentPlayerTag)
	FString CurrentStandTag;
	UPROPERTY(ReplicatedUsing = OnRep_CurrentPlayerTag)
	FGameplayTag CurrentPlayerTag; //로코모션담김
	UPROPERTY(ReplicatedUsing = OnRep_CurrentPlayerTag)
	FGameplayTag CurrentLockTag; //로코모션담김
	UPROPERTY(ReplicatedUsing = OnRep_CurrentPlayerTag)
	FGameplayTagContainer AbilityTagContainer; //공격중인지, 인트로중인지 태그
	UPROPERTY(ReplicatedUsing = OnRep_CurrentPlayerTag)
	bool bLookingRight{ true}; //오른쪽을 보고있는지
	UPROPERTY(ReplicatedUsing = OnRep_CurrentPlayerTag)
	bool bCheckTickCrouch{ false }; // crouch 상태인지
	UPROPERTY(ReplicatedUsing = OnRep_CurrentPlayerTag)
	bool bOnBuffering{ false };
	UPROPERTY(ReplicatedUsing = OnRep_CurrentPlayerTag)
	bool bBlocking{ false };

	UPROPERTY(ReplicatedUsing = OnRep_CurrentPlayerTag)
	FVector HitStopVector;

	FGameplayTag LastSentPlayerTag; //로코모션담김
	FTimerHandle HitStopTimerHandle;
public:
	UPROPERTY(ReplicatedUsing = OnRep_CurrentPlayerTag)
	FString CurrentMontageName;

protected:
	UPROPERTY(ReplicatedUsing = OnRep_CurrentPlayerTag, EditAnywhere, BlueprintReadWrite, Category = "Type")
	ECharacterType Type;

private:
	
};
