// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/AuraInputConfig.h"

const UInputAction* UAuraInputConfig::GetInputAction(const FGameplayTag AbilityTag, const bool bLogError = false)
{
	for (FAuraInputAction AuraInputAction : InputActions)
	{
		if (AuraInputAction.InputAction && AuraInputAction.AbilityTag == AbilityTag)
		{
			return AuraInputAction.InputAction;
		}
	}
	if (bLogError)
	{
		UE_LOG(LogTemp,Error,TEXT("Can not get InputAction with tag [%s] in AuraInputConfig"),*AbilityTag.ToString());
		
	}
	return nullptr;
	
}
