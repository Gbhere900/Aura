// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "AuraAbilitySystermLibrary.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilitySystermLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintPure,Category="AbilitySystemLibrary|OverlayWidgetController")
	static const UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContext);

	UFUNCTION(BlueprintPure,Category="AbilitySystemLibrary|AttributeMenuWidgetController")
	static UAttributeMenuWidgetController* GetAttributeSetWidgetController(const UObject* WorldContext);
};
