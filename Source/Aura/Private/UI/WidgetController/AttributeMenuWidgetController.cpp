// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAttributeSet.h"

void UAttributeMenuWidgetController::BindCallBackToDependences()
{
	
}

void UAttributeMenuWidgetController::BoardcastInitialAttribute() const
{
	FAuraAttributeInfo Info = AttributeInfo->GetAttributeInfo(FAuraGameplayTags::Get().Attributes_Primary_Strength,true);
	Info.AttributeValue = Cast<UAuraAttributeSet>(AttributeSet)->GetStrength();
	Info.AttributeValue = Cast<UAuraAttributeSet>(AttributeSet)->GetStrength();
	AttributeInfoDelegate.Broadcast(Info);
}
