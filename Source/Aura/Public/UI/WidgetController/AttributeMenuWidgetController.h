// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/AuraAttributeInfo.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AttributeMenuWidgetController.generated.h"

/**
 * 
 */

struct FGameplayAttribute;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoSignature, const FAuraAttributeInfo&, AuraAttributeInfo);

UCLASS(BlueprintType,Blueprintable)
class AURA_API UAttributeMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:
	virtual void BindCallBackToDependences() override;

	UFUNCTION(BlueprintCallable)
	virtual void BoardcastInitialAttribute() override;

	UPROPERTY(BlueprintAssignable,Category="GAS|Attribute")
	FAttributeInfoSignature AttributeInfoDelegate;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAttributeInfo> AttributeInfo;
	
private:
	void BroadcastAttributeInfo(const FGameplayTag& GameplayTag,const FGameplayAttribute& GameplayAttribute); 
	
};
