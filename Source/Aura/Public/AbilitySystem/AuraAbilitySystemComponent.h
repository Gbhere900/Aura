// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

/**
 * 
 */
DECLARE_MULTICAST_DELEGATE_OneParam(FEffectTags,const FGameplayTagContainer&);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAbilityChangedSignature);
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

	void AddInitialGameplayAbilities(TArray<TSubclassOf<UGameplayAbility>> GameplayAbilities);

	bool HasInitializeAbility = false; //
	
	//标记为BlueprintAssignable并且是UPROPERTY才可以在蓝图被绑定
	UPROPERTY(BlueprintAssignable)
	FAbilityChangedSignature OnAbilityChangedDelegate;
protected:
	UFUNCTION(Client,Reliable)		//为了解决客户端不显示应用效果的UI而写的，但是具体怎么实现
	void ClientEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& GameplayEffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle);

	//为什么是重写这个函数来实现客户端技能UI的同步？这个函数应该是负责能力激活的同步的啊？
	virtual void OnRep_ActivateAbilities() override;

};
