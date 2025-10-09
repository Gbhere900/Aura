// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"


#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"

void UOverlayWidgetController::BoardcastInitialAttribute() 
{
	Super::BoardcastInitialAttribute();
	const UAuraAttributeSet* AuraAttributeSet = Cast<UAuraAttributeSet> (AttributeSet);
	OnHealthChanged.Broadcast(AuraAttributeSet->Health.GetCurrentValue());
	OnMaxHealthChanged.Broadcast(AuraAttributeSet->MaxHealth.GetCurrentValue());
	OnManaChanged.Broadcast(AuraAttributeSet->Mana.GetCurrentValue());
	OnMaxManaChanged.Broadcast(AuraAttributeSet->MaxMana.GetCurrentValue());
}

void UOverlayWidgetController::BindCallBackToDependences()
{
	Super::BindCallBackToDependences();
	UAuraAttributeSet* AuraAttributeSet = Cast<UAuraAttributeSet> (AttributeSet); 
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetHealthAttribute())
	.AddLambda([this](const FOnAttributeChangeData& Health)
	{
		OnHealthChanged.Broadcast(Health.NewValue);
	});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxHealthAttribute())
	.AddLambda([this](const FOnAttributeChangeData& MaxHealth)
	{
		OnMaxHealthChanged.Broadcast(MaxHealth.NewValue);
	});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetManaAttribute())
	.AddLambda([this](const FOnAttributeChangeData& Mana)
	{
		OnManaChanged.Broadcast(Mana.NewValue);
	});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxManaAttribute())
	.AddLambda([this](const FOnAttributeChangeData& MaxMana)
	{
		OnMaxManaChanged.Broadcast(MaxMana.NewValue);
	});
	
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->EffectTags.AddLambda(
	[this](const FGameplayTagContainer& GameplayTagContainer)
	{
		for (const FGameplayTag &Tag : GameplayTagContainer)
		{
			FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
			if (Tag.MatchesTag(MessageTag))
			{
				const FUIWidgetRow *Row = GetDataTableRowByName<FUIWidgetRow>(DataTable,Tag);
				MessageWidgetRowDelegate.Broadcast(*Row);
			}
		}
	}
	);
}


