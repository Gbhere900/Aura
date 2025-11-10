// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GameplayAbility/AuraDamageGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

void UAuraDamageGameplayAbility::ApplyEffectToTarget(UAbilitySystemComponent* TargetASC)
{
	UAbilitySystemComponent * SourceASC =GetAbilitySystemComponentFromActorInfo();
	const FGameplayEffectSpecHandle GameplayEffectSpecHandle = SourceASC->MakeOutgoingSpec(DamageEffect,GetAbilityLevel(),SourceASC->MakeEffectContext());
	for (auto& Pair : DamageTypes)
	{
		const float DamageValue = Pair.Value.GetValueAtLevel(GetAbilityLevel());
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(GameplayEffectSpecHandle,Pair.Key,DamageValue);
		SourceASC->ApplyGameplayEffectSpecToTarget(*GameplayEffectSpecHandle.Data.Get(),TargetASC);
	}
}
