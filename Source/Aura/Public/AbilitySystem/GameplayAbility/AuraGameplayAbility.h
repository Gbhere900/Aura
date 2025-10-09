// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AuraGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
public :
	UPROPERTY(EditDefaultsOnly,Category="Input") // 如果要做键位设置是不是要改成蓝图可读写？
	FGameplayTag InputActionTag;
};
