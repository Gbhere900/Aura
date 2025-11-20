// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AbilityInfoData.generated.h"

USTRUCT(BlueprintType)
struct FAbilityInfos
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FGameplayTag AbilityTag;

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag InputTag; 
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TObjectPtr<UMaterialInterface> BackgroundImage;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TObjectPtr<UTexture2D> IconImage;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FGameplayTag CoolDownTag;
};
/**
 * 
 */
UCLASS(BlueprintType)
class AURA_API UAbilityInfoData : public UDataAsset
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TArray<FAbilityInfos> AbilityInfos;

public:
	FAbilityInfos GetAbilityInfo(const FGameplayTag& AbilityTag,const bool bLogIfNotFound);
};
