// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "AbilitySystem/Data/AuraInputConfig.h"
#include "AuraEnhancedInputComponent.generated.h"

class UAuraInputConfig;
/**
 * 
 */
UCLASS()
class AURA_API UAuraEnhancedInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	template<class UserClass,typename PressedFuncType,typename HeldFuncType,typename RelesedFuncType>
	void BindAbilityInputAction(UAuraInputConfig* AuraInputConfig,UserClass* Object,PressedFuncType PressedFunc,HeldFuncType HeldFunc,RelesedFuncType RelesedFunc);
};

template <class UserClass, typename PressedPuncType, typename HeldFuncType, typename RelesedFuncType>
void UAuraEnhancedInputComponent::BindAbilityInputAction(UAuraInputConfig* AuraInputConfig, UserClass* Object,
	PressedPuncType PressedFunc, HeldFuncType HeldFunc, RelesedFuncType RelesedFunc)					//为什么不用函数指针而是模版类型
{
	check(AuraInputConfig)
	for (const auto & AbilityInputAction : AuraInputConfig->InputActions)
	{
		if (AbilityInputAction.InputAction && AbilityInputAction.AbilityTag.IsValid())
		{
			if (PressedFunc)
			{
				BindAction(AbilityInputAction.InputAction,ETriggerEvent::Started,Object, PressedFunc,AbilityInputAction.AbilityTag);
			}
			if (HeldFunc)
			{
				BindAction(AbilityInputAction.InputAction,ETriggerEvent::Triggered,Object, HeldFunc,AbilityInputAction.AbilityTag);
			}
			if (RelesedFunc)
			{
				BindAction(AbilityInputAction.InputAction,ETriggerEvent::Canceled,Object, RelesedFunc,AbilityInputAction.AbilityTag);
			}
		}
	}
}
