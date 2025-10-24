// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "AuraProjectile.generated.h"


struct FGameplayEffectSpecHandle;
class UProjectileMovementComponent;
class UNiagaraSystem;

UCLASS()
class AURA_API AAuraProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AAuraProjectile();
	
	UPROPERTY(BlueprintReadWrite,meta=(ExposeOnSpawn = true))
	FGameplayEffectSpecHandle GameplayEffectSpec;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> SphereComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;
	virtual void BeginPlay() override;


	UFUNCTION()
	void OnSphereBeginOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	virtual void Destroyed() override;

	UPROPERTY(EditDefaultsOnly)
	float LifeTime = 5;



private:
	
	UPROPERTY(EditDefaultsOnly,Category="Audio")
	TObjectPtr<USoundBase> HitSound;

	UPROPERTY(EditDefaultsOnly,Category="Audio")
	TObjectPtr<USoundBase> LoopingSound;

	UPROPERTY(EditDefaultsOnly,Category="Particle")
	TObjectPtr<UNiagaraSystem> HitParticle;

	TObjectPtr<UAudioComponent> LoopingAudioComponent;
	
	

};
