#pragma once

#include "CoreMinimal.h"
#include "AbilityBase.h"
#include "TestFireball.generated.h"

UCLASS()
class GAMECORE_API UTestFireball : public UAbilityBase
{
	GENERATED_BODY()

public:
	UTestFireball();
	
public:
	virtual void Initialize(ACharacter* InOwner) override;
	virtual bool CanActivate() override;
	virtual void Activate() override; 
	virtual void PlayMontage() override; 
	virtual void OnAbilityFinished() override; //어빌리티 사용이 끝나면 사용한 변수들 초기화, Initialization of variables used after the use of abilities

};
