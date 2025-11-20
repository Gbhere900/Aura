// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"


#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/LevelInfo.h"
#include "Player/AuraPlayerState.h"

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
	AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState> (PlayerState);
	AuraPlayerState->OnLevelChangedDelegate.AddUObject(this,&UOverlayWidgetController::OnLevelChanged);
	AuraPlayerState->OnXPChangedDelegate .AddUObject(this,&UOverlayWidgetController::OnXPChanged);
	
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

	if (UAuraAbilitySystemComponent* ASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
	{
		//记住这个语法
		ASC->OnAbilityChangedDelegate.AddDynamic(this,&UOverlayWidgetController::OnChangeAbilities);

		if (ASC->HasInitializeAbility)
		{
			OnChangeAbilities();
		}
	}



	
}

void UOverlayWidgetController::OnChangeAbilities()
{
	if (UAuraAbilitySystemComponent* ASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
	{
		FScopedAbilityListLock ActiveScopeLock(*ASC);
		for (auto& AbilitySpec: ASC->GetActivatableAbilities())
		{
			FAbilityInfos Info;
			//为什么这里要加一个Get ?因为不加也不错
			for (FGameplayTag GamePlayTag : AbilitySpec.Ability.Get()->AbilityTags)
			{
				if (GamePlayTag.MatchesTag(FGameplayTag::RequestGameplayTag("Ability")))
				{
					Info = AbilityInfo->GetAbilityInfo(GamePlayTag,true);
						
					break;
				}
			}

			//能力和标签的关系是什么？上面获取的标签和这里获取的标签有什么区别
			for (FGameplayTag InputTag : AbilitySpec.DynamicAbilityTags)
			{
				if (InputTag.MatchesTag(FGameplayTag::RequestGameplayTag("InputTag")))
				{
					Info.InputTag = InputTag;
				}
			}
			OnAbilityChangedDelegate.Broadcast(Info);	
		}
	}
	
}

void UOverlayWidgetController::OnXPChanged()
{
	AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState>(PlayerState);
	ULevelInfo* LevelInfo = AuraPlayerState->LevelInfo;
	int Level = LevelInfo->GetLevelByXP(AuraPlayerState->GetXP());
	int XP = AuraPlayerState->GetXP();
	int MaxLevel = LevelInfo->LevelInformations.Num() - 1;
	if (Level >= MaxLevel)
	{
		OnMaxHealthChanged.Broadcast(1);
		return ;
	}
	float CurrentLevelXP  = LevelInfo->LevelInformations[Level].LevelXP;
	float NextLevelXP = LevelInfo->LevelInformations[Level].LevelXP;
	float SumXP = NextLevelXP - CurrentLevelXP;
	float Percent =  (XP - CurrentLevelXP )/ SumXP;
	OnXPChangedDelegate.Broadcast(Percent);
}

void UOverlayWidgetController::OnLevelChanged()
{
	// TODO: OnLevelChangedDelegate.Broadcast();
}


