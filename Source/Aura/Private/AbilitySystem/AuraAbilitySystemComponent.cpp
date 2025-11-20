// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AuraGameplayTags.h"
#include "NativeGameplayTags.h"
#include "AbilitySystem/GameplayAbility/AuraGameplayAbility.h"


void UAuraAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();
	//按我的理解在BeginPlay调用AbiliyuActoeInfoSet也还是可以的
	AbilityActorInfoSet();
}

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToTarget.AddUObject(this,&UAuraAbilitySystemComponent::ClientEffectApplied);
	//GEngine->AddOnScreenDebugMessage(-1,10,FColor::Orange,FAuraGameplayTags::Get().Attributes_Secondary_Armor.ToString());
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

void UAuraAbilitySystemComponent::AddInitialGameplayAbilities(TArray<TSubclassOf<UGameplayAbility>> GameplayAbilities)
{
	for (TSubclassOf<UGameplayAbility> Ability : GameplayAbilities)
	{

		FGameplayAbilitySpec GameplayAbilitySpec = FGameplayAbilitySpec(Ability,1);
		if (const UAuraGameplayAbility* AuraGameplayAbility = Cast<UAuraGameplayAbility>(GameplayAbilitySpec.Ability))
		{
			GameplayAbilitySpec.DynamicAbilityTags.AddTag(AuraGameplayAbility->InputActionTag);
		}
		GiveAbility(GameplayAbilitySpec);
	}
	HasInitializeAbility = true;
	OnAbilityChangedDelegate.Broadcast();
	
}

//这个函数看名字应该是在激活能力时，同步到客户端才触发的。但这里的逻辑更像是获得能力同步到客户端触发的
//而且能力的同步逻辑是什么？
void UAuraAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();
	OnAbilityChangedDelegate.Broadcast();
	HasInitializeAbility = true;
}

void UAuraAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& GameplayEffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle)
{
	FGameplayTagContainer GameplayTagContainer;
	GameplayEffectSpec.GetAllAssetTags(GameplayTagContainer);
	EffectTags.Broadcast(GameplayTagContainer);

	
}
