// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystermLibrary.h"

#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Game/AuraGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"

class AAuraPlayerState;

//蓝图函数库存在的必要性？感觉直接在c++类中或者蓝图中直接实现也没问题1
const UOverlayWidgetController* UAuraAbilitySystermLibrary::GetOverlayWidgetController(const UObject* WorldContext)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContext,0))
	{
		AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>();
		UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
		UAttributeSet* AS = PS->GetAttributeSet();
		if (AAuraHUD* HUD = Cast<AAuraHUD>(PC->GetHUD()))
		{
			FWidgetControllerParm WidgetControllerParm(PC,PS,ASC,AS);
			return HUD->GetOverlayWidgetController(WidgetControllerParm);
		}
	}
	return nullptr;
}

UAttributeMenuWidgetController* UAuraAbilitySystermLibrary::GetAttributeSetWidgetController(const UObject* WorldContext)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContext,0))
	{
		AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>();
		UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
		UAttributeSet* AS = PS->GetAttributeSet();
		if (AAuraHUD* HUD = Cast<AAuraHUD>(PC->GetHUD()))
		{
			FWidgetControllerParm WidgetControllerParm(PC,PS,ASC,AS);
			return HUD->GetAttributeSetWidgetController(WidgetControllerParm);
		}
	}
	return nullptr;
}

void UAuraAbilitySystermLibrary::InitializeCharacterAttributeByClass(const UObject* WorldContextObject,const ECharacterClass& CharacterClass,const int& Level, UAbilitySystemComponent* ASC)
{
	// AAuraGameModeBase* AuraGameModeBase = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	// if (!AuraGameModeBase)
	// 	return;
	UCharacterClassInfo* CharacterClassInfo =  Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject))->CharacterClassInfo;

	FGameplayEffectContextHandle GameplayEffectContextHandle = ASC->MakeEffectContext();
	GameplayEffectContextHandle.AddSourceObject(ASC->GetAvatarActor());
	
	TSubclassOf<UGameplayEffect> PrimaryEffect = CharacterClassInfo->GetClassPrimaryEffect(CharacterClass);
	FGameplayEffectSpecHandle PrimaryGameplayEffectSpecHandle = ASC->MakeOutgoingSpec(PrimaryEffect,Level, GameplayEffectContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryGameplayEffectSpecHandle.Data.Get());

	TSubclassOf<UGameplayEffect> SecondaryEffect = CharacterClassInfo->SecondaryAttributeEffect;
	FGameplayEffectSpecHandle SecondaryGameplayEffectSpecHandle = ASC->MakeOutgoingSpec(SecondaryEffect,Level, GameplayEffectContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryGameplayEffectSpecHandle.Data.Get());

	TSubclassOf<UGameplayEffect> VitalEffect = CharacterClassInfo-> VitalAttributeEffect;
	FGameplayEffectSpecHandle  VitalGameplayEffectSpecHandle = ASC->MakeOutgoingSpec( VitalEffect,Level, GameplayEffectContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(* VitalGameplayEffectSpecHandle.Data.Get());
}

void UAuraAbilitySystermLibrary::InitializeGameplayAbility(const UObject* WorldContextObject, UAbilitySystemComponent* ASC)
{
	AAuraGameModeBase* AuraGameModeBase = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	for (const auto& GameplayAbility : AuraGameModeBase->CharacterClassInfo->CommonGameplayAbility)
	{
		ASC->GiveAbility(GameplayAbility);
	}
	
}


