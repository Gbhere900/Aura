// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AbilityInfo.generated.h"

USTRUCT()
struct FAbilityInfo
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FGameplayTag AbilityTag;

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag InputTag; 
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FImage BackgroundImage;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FImage IconImage;
};
/**
 * 
 */
UCLASS()
class AURA_API UAbilityInfo : public UDataAsset
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TArray<FAbilityInfo> AbilityInfos;

public:
	FAbilityInfo GetAbilityInfo(const FGameplayTag& AbilityTag,const bool bLogIfNotFound);
};
