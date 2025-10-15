// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TargetDataUnderMouse.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMousetargetDataSignature,const FGameplayAbilityTargetDataHandle&, DataHandle);
/**
 * 
 */
UCLASS()
class AURA_API UTargetDataUnderMouse : public UAbilityTask		//AbilityTask存在的意义？现在还没觉得用ABiltyTAsk获取鼠标位置的必要性，做完这章再回头看
{
	GENERATED_BODY()
	UFUNCTION(BlueprintCallable,Category="Ability|Tasks",meta = (DisplayName ="TargetDataUnderMouse",HidePin = "OwningAbility",DefaultToSelf = "OwningAbility",BlueprintInternalUseOnly = "true"))
	static UTargetDataUnderMouse* CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility);

	UPROPERTY(BlueprintAssignable)
	FMousetargetDataSignature ValidData;

private:
	virtual void Activate() override;

	void SendMouseCursorData();
	void OnTargetDataReplicateCallback(const FGameplayAbilityTargetDataHandle& DataHandle,FGameplayTag AvtivationTag);
};
