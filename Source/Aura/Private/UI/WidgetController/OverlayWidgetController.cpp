// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/AuraAttributeSet.h"

void UOverlayWidgetController::BoardcastInitialAttribute() const
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
	.AddUObject(this,&UOverlayWidgetController::HealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxHealthAttribute())
	.AddUObject(this,&UOverlayWidgetController::MaxHealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetManaAttribute())
	.AddUObject(this,&UOverlayWidgetController::ManaChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxManaAttribute())
	.AddUObject(this,&UOverlayWidgetController::MaxManaChanged);
}

void UOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Health)
{
	OnHealthChanged.Broadcast(Health.NewValue);
}

void UOverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& MaxHealth)
{
	OnMaxHealthChanged.Broadcast(MaxHealth.NewValue);
}

void UOverlayWidgetController::ManaChanged(const FOnAttributeChangeData& Mana)
{
	OnManaChanged.Broadcast(Mana.NewValue);
}

void UOverlayWidgetController::MaxManaChanged(const FOnAttributeChangeData& MaxMana)
{
	OnMaxManaChanged.Broadcast(MaxMana.NewValue);
}
