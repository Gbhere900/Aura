// Fill out your copyright notice in the Description page of Project Settings.


#include "BlueprintAsyncAction/WaitAbilityCD.h"

#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"

UWaitAbilityCD* UWaitAbilityCD::WaitForAbilityCD(const FGameplayTag& CooldownTag, UAbilitySystemComponent* ASC)
{
	UWaitAbilityCD* WaitAbilityCD = NewObject<UWaitAbilityCD>();
	WaitAbilityCD->CooldownTag = CooldownTag;
	WaitAbilityCD->ASC = ASC;

	ASC->RegisterGameplayTagEvent(CooldownTag,EGameplayTagEventType::NewOrRemoved).AddUObject(WaitAbilityCD,&UWaitAbilityCD::OnGameplayTagChanged);
	return WaitAbilityCD;
}

void UWaitAbilityCD::EndTask()
{
	ASC->RegisterGameplayTagEvent(CooldownTag,EGameplayTagEventType::NewOrRemoved).RemoveAll(this);
	SetReadyToDestroy();
	MarkAsGarbage();
}

void UWaitAbilityCD::OnGameplayTagChanged(const FGameplayTag, int32 NewVal)
{
	if (NewVal > 0)
	{
		
		//ActiveGameplayEffectQuery和GameplayEffectQuery有什么区别
		FGameplayEffectQuery GameplayEffectQuery = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTag.GetSingleTagContainer());

		//这里GameEffectQuery是怎么工作的
		TArray<float>TimeRemaining =  ASC->GetActiveEffectsTimeRemaining(GameplayEffectQuery);
		CoolDownBeginDelegate.Broadcast(TimeRemaining[0]);
	}
	else
	{
		CoolDownEndDelegate.Broadcast();
	}
}
