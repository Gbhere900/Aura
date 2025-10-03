// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystermLibrary.h"

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


