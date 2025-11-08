// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/CharacterClassInfo.h"

TSubclassOf<UGameplayEffect> UCharacterClassInfo::GetCharacterClassPrimaryEffect(const ECharacterClass& CharacterClass)
{
	return Class_PrimaryAttributeEffectMap.FindChecked(CharacterClass).PrimaryAttributeEffect;
}

TArray<TSubclassOf<UGameplayAbility>> UCharacterClassInfo::GetCharacterClassAbilities(const ECharacterClass& CharacterClass)
{
	return Class_PrimaryAttributeEffectMap.FindChecked(CharacterClass).CharacterAbilities;
}
