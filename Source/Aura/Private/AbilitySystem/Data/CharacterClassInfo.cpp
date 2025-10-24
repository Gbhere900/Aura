// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/CharacterClassInfo.h"

TSubclassOf<UGameplayEffect> UCharacterClassInfo::GetClassPrimaryEffect(const ECharacterClass& CharacterClass)
{
	return Class_PrimaryAttributeEffectMap.FindChecked(CharacterClass).PrimaryAttributeEffect;
}
