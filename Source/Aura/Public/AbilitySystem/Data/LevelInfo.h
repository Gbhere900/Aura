// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LevelInfo.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FLevelInformation
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int LevelXP;
	
	
};


UCLASS()
class AURA_API ULevelInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FLevelInformation> LevelInformations;

	UFUNCTION(BlueprintCallable)
	int GetLevelByXP(int XP);
};
