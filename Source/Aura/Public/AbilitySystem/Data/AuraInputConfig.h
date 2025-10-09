// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AuraInputConfig.generated.h"

/**
 * 
 */

class UInputAction;

USTRUCT(BlueprintType)
struct FAuraInputAction			//感觉可以换成另一个更加可读的名称，以及为什么不用map而是结构体
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	 FGameplayTag AbilityTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	const UInputAction* InputAction = nullptr;
};

UCLASS()
class AURA_API UAuraInputConfig : public UDataAsset
{
	GENERATED_BODY()
public:
	const UInputAction* GetInputAction(FGameplayTag AbilityTag, const bool bLogError);
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)		//视频里是放在public
	TArray<FAuraInputAction> InputActions;
	
};
