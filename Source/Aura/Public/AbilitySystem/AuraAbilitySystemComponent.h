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
	UFUNCTION(Client,Reliable)		//为了解决客户端不显示应用效果的UI而写的，但是具体怎么实现
	void ClientEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& GameplayEffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle);

};
