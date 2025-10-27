// ReSharper disable once UnrealHeaderToolError
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffect.h"
#include "AbilitySystem/CombatInterface.h"
#include "AuraCharacterBase.generated.h"

class UAttributeSet;
class UAuraAbilitySystemComponent;
class UAuraAttributeSet;

UCLASS(Abstract)
class AURA_API AAuraCharacterBase : public ACharacter,public IAbilitySystemInterface,public ICombatInterface
{
	GENERATED_BODY()

public:
	AAuraCharacterBase();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual TObjectPtr<UAttributeSet> GetAttributeSet() const {return AttributeSet;}
	void AddGameplayAbilities();
	
protected:
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere,Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category="Attribute")
	TSubclassOf<UGameplayEffect> InitialPrimaryGamePlayEffect;

	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category="Attribute")
	TSubclassOf<UGameplayEffect> InitialSecondaryGamePlayEffect;
	
	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category="Attribute")
	TSubclassOf<UGameplayEffect> InitialVitalGamePlayEffect;

	void virtual InitAbilityActorInfo();
	void virtual InitialAttributeSet();
	void ApplyGameplayEffectSpecToSelf(const TSubclassOf<UGameplayEffect> GameplayEffect,const int Level) const;

	virtual FTransform GetSocketTransform() override;

	virtual int GetLevel() override;

	virtual void Die() override;

	UFUNCTION(NetMulticast,Reliable)
	void MulticastHandleDeath();

private:
	UPROPERTY(EditAnywhere,Category="GameplayAbilities")
	TArray<TSubclassOf<UGameplayAbility>> GameplayAbilities; 
	
};
