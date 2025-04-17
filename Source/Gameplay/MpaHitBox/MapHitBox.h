// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "MapHitBox.generated.h"

UCLASS()
class GAMEPLAY_API AMapHitBox : public AActor
{
	GENERATED_BODY()

public:
	AMapHitBox();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
public:
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnCameraEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_SpawnEffect(const FVector& SpawnLoc, const FRotator& SpawnRot);
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UBoxComponent* CameraHitBoxComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UBoxComponent* HitBoxComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ACharacter> CharacterClass;

	UPROPERTY(EditAnywhere, Category = "CameraEffect")
	TSubclassOf<UCameraShakeBase> RespawnCameraShake;

	UPROPERTY(EditAnywhere, Category="Effects")
	UNiagaraSystem* FallOffEffect;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Respawn")
	APlayerStart* RespawnTarget;
	
	FTimerHandle RespawnTimerHandle;

private:
	void OnCameraShake();
};

