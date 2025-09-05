// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraEnemy.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"

void AAuraEnemy::HighlightEnemy()
{
	GetMesh()->SetRenderCustomDepth(true);
}

void AAuraEnemy::UnHighLightEnemy()
{
	GetMesh()->SetRenderCustomDepth(false);
}


AAuraEnemy::AAuraEnemy()
{
	AbilitySystemComponent->CreateDefaultSubobject<UAuraAbilitySystemComponent>(FName("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet->CreateDefaultSubobject<UAuraAttributeSet>(FName("AttributeSet"));
}

void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();
	UnHighLightEnemy();
	
	AbilitySystemComponent->InitAbilityActorInfo(this,this);
}






