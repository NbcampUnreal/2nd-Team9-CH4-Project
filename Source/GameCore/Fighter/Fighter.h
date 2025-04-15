// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayTagContainer.h"
#include "Fighter.generated.h"

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

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Landed(const FHitResult& Hit) override;
	
public:
	UFUNCTION()
	void ImSleepy(const FString& MessageType, UObject* Payload);
	UFUNCTION()
	void Move(const FInputActionValue& InputValue);
	UFUNCTION()
	virtual void StartJump(const FInputActionValue& InputValue);
	UFUNCTION()
	void ChangeLook();
	void SetCheckTickCrouch(); 
	void SetBuffering(const bool bBuffering) { bOnBuffering = bBuffering; }
	UFUNCTION(BlueprintCallable)
	void SetCurrentStandTag(const FString& InStandTag) { CurrentStandTag = InStandTag; }
	UFUNCTION(BlueprintCallable)
	FGameplayTag GetGameplayTag() const { return CurrentPlayerTag; }

	UFUNCTION(BlueprintCallable)
	FGameplayTagContainer& GetCurrentTags();

	UFUNCTION(BlueprintCallable)
	bool GetPlayerLookingRight() const { return bLookingRight; }
	
	UFUNCTION(BlueprintCallable)
	void SetGameplayTag(const FGameplayTag& GameplayTag) { CurrentPlayerTag = GameplayTag; }
	UFUNCTION(BlueprintCallable)
	void UnlockedTag();
	void RefreshlockTag();
	void LockTag();
	void SetBlocking(const bool bInBlocking) { bBlocking = bInBlocking; }
	void AddAttackTag() { AbilityTagContainer.AddTag(AttackTag); }
	bool GetBuffering();
	void StartBlocking(const FInputActionValue& InputActionValue);
	void EndBlocking(const FInputActionValue& InputActionValue);
	void RemoveAttackTag() { AbilityTagContainer.RemoveTag(AttackTag); };
	void SetChangeBaseTag() { CurrentPlayerTag = BaseTag; };
	void SetChangeStandTag();
	void SetIdleTag() { CurrentPlayerTag = IdleTag; };
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	UNiagaraSystem* NiagaraEffect;
	TArray<UNiagaraComponent*> NiagaraComponents;
	
	/* Define 에 옮겨놔야할듯 */
	static FGameplayTag AttackTag;
	static FGameplayTag BaseTag;
	static FGameplayTag JumpTag;
	static FGameplayTag IdleTag;
	static FGameplayTag LandTag;
	static FGameplayTag HitTag;
	static FGameplayTag CrouchTag;
	static FGameplayTag BlockingTag;
private: /* 카메라 완성되면 지워야함*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;
private:
	// 앉아있는지 서있는지 초기화 할때 쓰이는 변수
	FString CurrentStandTag;
	FGameplayTag CurrentPlayerTag; //로코모션담김
	FGameplayTag CurrentLockTag; //로코모션담김
	FGameplayTagContainer AbilityTagContainer; //공격중인지, 인트로중인지 태그
	
	bool bLookingRight{ true}; //오른쪽을 보고있는지
	bool bCheckTickCrouch{ false }; // crouch 상태인지
	bool bOnBuffering{ false };
	bool bBlocking{ false };

public:
	FString CurrentMontageName;
};

