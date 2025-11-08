// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "AuraAbilitySystermLibrary.generated.h"

struct FGameplayEffectContextHandle;
struct FGameplayEffectContext;
class UCharacterClassInfo;
enum class ECharacterClass : uint8;
/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilitySystermLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure,Category="AbilitySystemLibrary|OverlayWidgetController")
	static const UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContext);

	UFUNCTION(BlueprintPure,Category="AbilitySystemLibrary|AttributeMenuWidgetController")
	static UAttributeMenuWidgetController* GetAttributeSetWidgetController(const UObject* WorldContext);

	UFUNCTION(BlueprintCallable,Category="AbilitySystemLibrary|CharacterClass")
	static void InitializeCharacterAttributeByClass(const UObject* WorldContextObject, const ECharacterClass& CharacterClass,const int& Level, UAbilitySystemComponent* ASC);

	UFUNCTION(BlueprintCallable,Category="AbilitySystemLibrary|CharacterClass")
	static void InitializeGameplayAbilityByClass(const UObject* WorldContextObject,UAbilitySystemComponent* ASC,ECharacterClass CharacterClass);

	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);

	/*
	 *下面这四个函数有必要在函数库里写一遍吗，感觉其实没必要
	 */
	
	UFUNCTION(BlueprintPure,Category="AbilitySystemLibrary|GameEffectContext")
	static bool GetIsBlocked(const FGameplayEffectContextHandle& GameplayEffectContextHandle);

	UFUNCTION(BlueprintPure,Category="AbilitySystemLibrary|GameEffectContext")
	static bool GetIsCriticalHit(const FGameplayEffectContextHandle& GameplayEffectContextHandle);

	UFUNCTION(BlueprintCallable,Category="AbilitySystemLibrary|GameEffectContext")
	static void SetIsCriticalHit(FGameplayEffectContextHandle& GameplayEffectContextHandle,bool b);

	UFUNCTION(BlueprintCallable,Category="AbilitySystemLibrary|GameEffectContext")
	static void SetIsBlocked(FGameplayEffectContextHandle& GameplayEffectContextHandle,bool b);

	UFUNCTION(BlueprintCallable)
	static void GetLivePlayerWithinRadius(UPARAM()TArray<AActor*>& OutActors,const TArray<AActor*>& IgnoreActors,const FVector& Center,const float& Radius,const UObject* WorldContextObject);
};

