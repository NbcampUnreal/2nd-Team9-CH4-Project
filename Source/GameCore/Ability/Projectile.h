// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityData/AnimRow.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class USphereComponent;
class UNiagaraComponent;
class UNiagaraSystem;

UCLASS()
class GAMECORE_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void Init(const FAnimRow AnimRow);

protected:
	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void SetLocation();
	
	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void OnNiagaraFinished();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	TArray<UNiagaraSystem*> NiagaraEffect;
	TMap<int32,UNiagaraComponent*> NiagaraComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	FAnimRow AnimInfo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	FVector VecDir;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	bool bLookingRight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTimerHandle DestroyTimerHandle;
};