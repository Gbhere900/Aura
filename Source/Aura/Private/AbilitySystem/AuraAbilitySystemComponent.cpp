// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AuraGameplayTags.h"
#include "NativeGameplayTags.h"


void UAuraAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();
	//按我的理解在BeginPlay调用AbiliyuActoeInfoSet也还是可以的
	AbilityActorInfoSet();
}

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToTarget.AddUObject(this,&UAuraAbilitySystemComponent::EffectApplied);
	GEngine->AddOnScreenDebugMessage(-1,10,FColor::Orange,FAuraGameplayTags::Get().Attributes_Secondary_Armor.ToString());
}

void UAuraAbilitySystemComponent::GameplayAbilityHeldFunc(FGameplayTag GameplayTag)
{
	for (auto& GameplayAbilitySpec : GetActivatableAbilities())
	{
		if (GameplayAbilitySpec.DynamicAbilityTags.HasTagExact(GameplayTag))
		{
			AbilitySpecInputPressed(GameplayAbilitySpec);
			if (!GameplayAbilitySpec.IsActive())
			{
				TryActivateAbility(GameplayAbilitySpec.Handle);
			}
		}
	}
}

void UAuraAbilitySystemComponent::GameplayAbilityRelesedFunc(FGameplayTag GameplayTag)
{
	for (auto& GameplayAbilitySpec : GetActivatableAbilities())
	{
		if (GameplayAbilitySpec.DynamicAbilityTags.HasTagExact(GameplayTag))
		{
			AbilitySpecInputReleased(GameplayAbilitySpec);
		}
	}
}

void UAuraAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& GameplayEffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle)
{
	FGameplayTagContainer GameplayTagContainer;
	GameplayEffectSpec.GetAllAssetTags(GameplayTagContainer);
	EffectTags.Broadcast(GameplayTagContainer);

	
}
