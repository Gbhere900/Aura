// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

/**
 * 
 */
DECLARE_MULTICAST_DELEGATE_OneParam(FEffectTags,const FGameplayTagContainer&);
UCLASS()
class AURA_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	void BeginPlay() override;
	void AbilityActorInfoSet();
	FEffectTags EffectTags;

	void GameplayAbilityHeldFunc(FGameplayTag GameplayTag);
	void GameplayAbilityRelesedFunc(FGameplayTag GameplayTag);
protected:
	void EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& GameplayEffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle);

};
