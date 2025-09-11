// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AuraEffectActor.generated.h"

class UGameplayEffect;

UCLASS()
class AURA_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	

	AAuraEffectActor();
	UFUNCTION(BlueprintCallable)
	virtual void ApplyEffectToTarget(AActor* TargetActor,TSubclassOf<UGameplayEffect> GamePlayEffectClass);

protected:

	virtual void BeginPlay() override;

private:
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(VisibleAnywhere)
	TSubclassOf<UGameplayEffect> GameplayEffectClass;

	

};
