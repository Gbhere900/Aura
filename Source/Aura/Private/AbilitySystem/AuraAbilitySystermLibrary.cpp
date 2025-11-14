// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystermLibrary.h"

#include "AuraAbilityTypes.h"
#include "AbilitySystem/CombatInterface.h"
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
	
	TSubclassOf<UGameplayEffect> PrimaryEffect = CharacterClassInfo->GetCharacterClassPrimaryEffect(CharacterClass);
	FGameplayEffectSpecHandle PrimaryGameplayEffectSpecHandle = ASC->MakeOutgoingSpec(PrimaryEffect,Level, GameplayEffectContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryGameplayEffectSpecHandle.Data.Get());

	TSubclassOf<UGameplayEffect> SecondaryEffect = CharacterClassInfo->SecondaryAttributeEffect;
	FGameplayEffectSpecHandle SecondaryGameplayEffectSpecHandle = ASC->MakeOutgoingSpec(SecondaryEffect,Level, GameplayEffectContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryGameplayEffectSpecHandle.Data.Get());

	TSubclassOf<UGameplayEffect> VitalEffect = CharacterClassInfo-> VitalAttributeEffect;
	FGameplayEffectSpecHandle  VitalGameplayEffectSpecHandle = ASC->MakeOutgoingSpec( VitalEffect,Level, GameplayEffectContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(* VitalGameplayEffectSpecHandle.Data.Get());
}

void UAuraAbilitySystermLibrary::InitializeGameplayAbilityByClass(const UObject* WorldContextObject, UAbilitySystemComponent* ASC,ECharacterClass CharacterClass)
{
	AAuraGameModeBase* AuraGameModeBase = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	for (const auto& GameplayAbility : AuraGameModeBase->CharacterClassInfo->CommonGameplayAbility)
	{
		ASC->GiveAbility(GameplayAbility);
	}

	for (const auto& GameplayAbility : AuraGameModeBase->CharacterClassInfo->GetCharacterClassAbilities(CharacterClass))
	{
		const int Characterlevel = CastChecked<ICombatInterface>(ASC->GetAvatarActor())->GetLevel();
		FGameplayAbilitySpec GameplayAbilitySpec(GameplayAbility,Characterlevel);
		ASC->GiveAbility(GameplayAbilitySpec);
	}
}

UCharacterClassInfo* UAuraAbilitySystermLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	AAuraGameModeBase* AuraGameModeBase = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (AuraGameModeBase)
	{
		return AuraGameModeBase->CharacterClassInfo;
	}
	return nullptr;
}



bool UAuraAbilitySystermLibrary::GetIsBlocked(const FGameplayEffectContextHandle& GameplayEffectContextHandle)
{
	//为什么下面要加static_cast 和*
	if (const FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<const FAuraGameplayEffectContext*>(GameplayEffectContextHandle.Get()))
	{
		return AuraGameplayEffectContext->IsBlockHit();
	}
	return false;
	
}



bool UAuraAbilitySystermLibrary::GetIsCriticalHit(const FGameplayEffectContextHandle& GameplayEffectContextHandle)
{
	//为什么下面要加static_cast 和*
	if (const FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<const FAuraGameplayEffectContext*>(GameplayEffectContextHandle.Get()))
	{
		return AuraGameplayEffectContext->IsCriticalHit();
	}
	return false;
}

//给蓝图用的SetIsCriticalHit/SetIsBlocked真的有必要吗？？
void UAuraAbilitySystermLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& GameplayEffectContextHandle,bool b)
{
	//为什么下面要加static_cast 和*
	if (FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<FAuraGameplayEffectContext*>(GameplayEffectContextHandle.Get()))
	{
		AuraGameplayEffectContext->SetIsCriticalHit(b);
	}
}

void UAuraAbilitySystermLibrary::SetIsBlocked(FGameplayEffectContextHandle& GameplayEffectContextHandle, bool b)
{
	//为什么下面要加static_cast 和*
	if (FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<FAuraGameplayEffectContext*>(GameplayEffectContextHandle.Get()))
	{
		AuraGameplayEffectContext->SetIsBlockHit(b);
	}
}

void UAuraAbilitySystermLibrary::GetLivePlayerWithinRadius(TArray<AActor*>& OutActors, const TArray<AActor*>& IgnoreActors, const FVector& Center, const float& Radius, const UObject* WorldContextObject)
{
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(IgnoreActors);

	TArray<FOverlapResult> Overlaps;
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		World->OverlapMultiByObjectType(Overlaps, Center, FQuat::Identity,
			FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects),
			FCollisionShape::MakeSphere(Radius), SphereParams);

		for (auto& OverlapResult : Overlaps)
		{
			if (bool ImplementInterface = OverlapResult.GetActor()->Implements<UCombatInterface>())
			{
				bool IsDead = ICombatInterface::Execute_IsDead(OverlapResult.GetActor());
				if (!IsDead)
				{
					OutActors.AddUnique(OverlapResult.GetActor());
				}
			}
		}
	}
}

bool UAuraAbilitySystermLibrary::IsFriendly(AActor* Actor1, AActor* Actor2)
{
	bool HasPlayerTag1 = Actor1->ActorHasTag("Player");
	bool HasPlayerTag2 = Actor2->ActorHasTag("Player");

	bool HasEnemyTag1 = Actor1->ActorHasTag("Enemy");
	bool HasEnemyTag2 = Actor2->ActorHasTag("Enemy");

	return (HasPlayerTag1 && HasPlayerTag2 || HasEnemyTag1 && HasEnemyTag2);
}






