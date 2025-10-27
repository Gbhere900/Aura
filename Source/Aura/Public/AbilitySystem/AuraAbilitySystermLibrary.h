// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "AuraAbilitySystermLibrary.generated.h"

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
	static void InitializeGameplayAbility(const UObject* WorldContextObject,UAbilitySystemComponent* ASC);
};
