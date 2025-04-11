// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
//Test
#include "GameplayTagContainer.h"
#include "Fighter.generated.h"

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

public:
	void IWantToSleep() const;

	UFUNCTION()
	void ImSleepy(const FString& MessageType, UObject* Payload);

	UFUNCTION()
	void Move(const FInputActionValue& InputValue);

	UFUNCTION()
	virtual void StartJump(const FInputActionValue& InputValue);

	UFUNCTION(BlueprintCallable)
	FGameplayTag GetGameplayTag() const { return CurrentPlayerTag; }

	UFUNCTION(BlueprintCallable)
	void SetGameplayTag(const FGameplayTag& GameplayTag) { CurrentPlayerTag = GameplayTag; };
	void AddAttackTag() { AbilityTagContainer.AddTag(AttackTag); };
	void RemoveAttackTag() { AbilityTagContainer.RemoveTag(AttackTag); };
	void SetChangeBaseTag() { CurrentPlayerTag = BaseTag; };
	void SetChangeStandTag();
private:
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	//USpringArmComponent* SpringArm;
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	//UCameraComponent* Camera;

	/*tw Test
	 * 단일 태그로 테스트해본결과
	 * 기본 상태를 애니메이션 블루프린트로 하고 블렌드로 어빌리티에 있는 몽타쥬를 섞고 싶었음
	 * 그게 확장성도 좋아보였으니 결론적으론 움직임 자체를 담고 있는건
	 * FGameplayTag CurrentPlayerTag; 를 쓰고
	 * 어빌리티 및 각종 게임상태를 담는건
	 * FGameplayTagContainer AbilityTagContainer; 로 관리하는게 맞아보임
	 * 컨테이너는 순서보장도 힘들어서 움직임 상태를 빠르게 전환시키기에는 애매함 
	 * 예시로 FGameplayTagContainer에 PlayerState.Base.Idle 을 PlayerState.Base.WalkForward 로 바꿀려면
	 * PlayerState.Base 하위에 있는걸 다 찾아서 받은다음 (아니면 하위에 있는걸 한번에 지우는게 잇나?)
	 * 반환된 Tag를 다시 컨테이너에 던져줘서 지워달라고 요청하고 새로운 PlayerState.Base.WalkForward 로 AddTag 해야됨
	 * 뭔가뭔가임 더 찾아봐야할듯
	 */
	static FGameplayTag AttackTag;
	static FGameplayTag BaseTag;
	static FGameplayTag JumpTag;
	
	FString CurrentStandTag;
	
	FGameplayTag CurrentPlayerTag; //로코모션담김
	//jh Test
public:
	FGameplayTagContainer AbilityTagContainer; //공격중인지, 인트로중인지 태그
	UFUNCTION(BlueprintCallable)
	FGameplayTagContainer& GetCurrentTags();
};