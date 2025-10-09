// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"

void UAttributeMenuWidgetController::BindCallBackToDependences()
{
	UAuraAttributeSet* AS = Cast<UAuraAttributeSet>(AttributeSet);
	for (auto& Pair:AS->TagToFunctionPointer)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda
		(
			[this, Pair, AS](FOnAttributeChangeData OnAttributeChangeData)
			{
				BroadcastAttributeInfo(Pair.Key,Pair.Value());
			}
		);
	}
	
}

void UAttributeMenuWidgetController::BoardcastInitialAttribute() 
{
	UAuraAttributeSet* AS = Cast<UAuraAttributeSet>(AttributeSet);
	for (auto& Pair : AS->TagToFunctionPointer)
	{
		BroadcastAttributeInfo(Pair.Key,Pair.Value());
	}
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& GameplayTag,const FGameplayAttribute& GameplayAttribute)
{
	FAuraAttributeInfo Info = AttributeInfo->GetAttributeInfo(GameplayTag,true);
	//Info.AttributeValue = GameplayAttribute.GetNumericValue(Cast<UAuraAttributeSet>(AttributeSet));
	Info.AttributeValue = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->GetNumericAttribute(GameplayAttribute); //上面注释掉的会得到错误的数值0
	//GEngine->AddOnScreenDebugMessage(-1,5,FColor::Red,FString::Printf(TEXT("%f"),Cast<UAuraAttributeSet>(AttributeSet)->Armor.GetCurrentValue()));
	//GEngine->AddOnScreenDebugMessage(-1,5,FColor::Red,FString::Printf(TEXT("%f"),Info.AttributeValue));
	AttributeInfoDelegate.Broadcast(Info);
}




