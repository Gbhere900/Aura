// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerState.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Net/UnrealNetwork.h"

AAuraPlayerState::AAuraPlayerState()
{
	//理解这里的网络同步设置很重要
	NetUpdateFrequency = 100;
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>(FName("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>(FName("AttributeSet"));
	//AbilitySystemComponent->AddAttributeSetSubobject<UAuraAttributeSet>(AttributeSet);

	
}

UAbilitySystemComponent* AAuraPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AAuraPlayerState::OnRep_Level()
{
	OnLevelChangedDelegate.Broadcast();
}

void AAuraPlayerState::OnRep_XP()
{
	OnXPChangedDelegate.Broadcast();
}

void AAuraPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AAuraPlayerState,Level);
	DOREPLIFETIME(AAuraPlayerState,XP);
}


