// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/GameplayAbility/AuraGameplayAbility.h"
#include "AuraSummonGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraSummonGameplayAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Summon")
	TArray<TSubclassOf<AActor>> SummonTarget;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Summon")
	float SpawnNum;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Summon")
	float SpawnDelay;

	UPROPERTY(EditDefaultsOnly,Category="Summon")
	float SpawnAngle;

	UPROPERTY(EditDefaultsOnly,Category="Summon")
	float MinSpawnDistance;

	UPROPERTY(EditDefaultsOnly,Category="Summon")
	float MaxSpawnDistance;

	TArray<FVector> SpawnLocations;

public:
	UFUNCTION(BlueprintCallable)
	TArray<FVector> GetSpawnLocations();
	
};
