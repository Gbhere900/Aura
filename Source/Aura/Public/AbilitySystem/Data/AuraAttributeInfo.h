// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AuraAttributeInfo.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FAuraAttributeInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	FGameplayTag GameplayTag = FGameplayTag();

	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	FText AttributeName = FText();

	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	FText AttributeDescription ;

	UPROPERTY(BlueprintReadOnly)
	float AttributeValue = 0.0f;
	
};



UCLASS()
class AURA_API UAttributeInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	TArray<FAuraAttributeInfo> AttributeInfos;

	FAuraAttributeInfo GetAttributeInfo(const FGameplayTag& GameplayTag,bool bLogNotFound);
};
