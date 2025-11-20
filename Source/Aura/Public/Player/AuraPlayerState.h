// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "GameFramework/PlayerState.h"
#include "AuraPlayerState.generated.h"

class ULevelInfo;
class UAttributeSet;
class UAuraAttributeSet;
class UAuraAbilitySystemComponent;

DECLARE_MULTICAST_DELEGATE(OnValChangedSignature);

/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerState : public APlayerState,public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	AAuraPlayerState();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	TObjectPtr<UAttributeSet> GetAttributeSet() const {return AttributeSet;};

	OnValChangedSignature OnLevelChangedDelegate;
	OnValChangedSignature OnXPChangedDelegate;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULevelInfo> LevelInfo;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 const GetXP() const {return XP;}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 const GetLevel() const {return Level;}
	
	UFUNCTION(BlueprintCallable)
	void AddLevel(int AddNum)
	{
		Level += AddNum;
		OnLevelChangedDelegate.Broadcast();
	};

	UFUNCTION(BlueprintCallable)
	void AddXP(int AddNum)
	{
		XP += AddNum;
		OnXPChangedDelegate.Broadcast();
	};

	void SetLevel(int NewVal)
	{
		Level = NewVal;
		OnLevelChangedDelegate.Broadcast();
	};

	UFUNCTION(BlueprintCallable)
	void SetXP(int NewVal)
	{
		XP = NewVal;
		OnXPChangedDelegate.Broadcast();
	};
	
protected:
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAuraAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAuraAttributeSet> AttributeSet;

	UFUNCTION()
	void OnRep_Level();
	UFUNCTION()
	void OnRep_XP();
	
	UPROPERTY(VisibleAnywhere,ReplicatedUsing= OnRep_Level)
	int32 Level =0;

	UPROPERTY(VisibleAnywhere,ReplicatedUsing= OnRep_XP)
	int32 XP = 0;



};
