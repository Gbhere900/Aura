// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "WaitAbilityCD.generated.h"

class UAbilitySystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCoolDownBeginSignature, float,CooldownTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCoolDownEndSignature);
/**
 *  这个异步类的创建有什么必要吗，实际上要完成CD的显示只要在控件里绑定到ASC的GameplayTag事件就行了（也许是为了实现MVC架构？）
 */
UCLASS(BlueprintType,meta =(ExposedAsyncProxy = "Async Task"))
class AURA_API UWaitAbilityCD : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public :
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly))
	static UWaitAbilityCD* WaitForAbilityCD(const FGameplayTag& CooldownTag,UAbilitySystemComponent* ASC);

	UPROPERTY(BlueprintAssignable)
	FCoolDownBeginSignature CoolDownBeginDelegate;
	UPROPERTY(BlueprintAssignable)
	FCoolDownEndSignature CoolDownEndDelegate;

	UFUNCTION(BlueprintCallable)
	void EndTask();
	
protected:
	FGameplayTag CooldownTag;
	UAbilitySystemComponent* ASC;

	void OnGameplayTagChanged(const FGameplayTag, int32 NewVal);
};
