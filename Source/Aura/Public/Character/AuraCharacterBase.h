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

	virtual UNiagaraSystem* GetBloodEffect_Implementation() override;

	virtual int GetFollowerNum_Implementation() override
	{
		return FollowerNum;
	};

	virtual void AddOrSubtractFollowerNum_Implementation(const int DeltaValue) override
	{
		FollowerNum += DeltaValue;
	};
protected:
	//为什么运动扭曲组件motion warpping 不在这里定义而是在Aura和EnemyBase蓝图里分别添加
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

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> MeshMaterialInstance;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> WeaponMaterialInstance;

	void virtual InitAbilityActorInfo();
	void virtual InitialAttributeSet();
	void ApplyGameplayEffectSpecToSelf(const TSubclassOf<UGameplayEffect> GameplayEffect,const int Level) const;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FString RightHandSocketName;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FString LeftHandSocketName;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FString WeaponSocketName;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FString TailSocketName;
	virtual FVector GetSocketLocation_Implementation(FGameplayTag GameplayTag) override;
	
	virtual int GetLevel() override;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="CharacterClassDefault")
	int Level = 1;

	UPROPERTY(EditDefaultsOnly,category="Death")
	TObjectPtr<USoundBase> DeathSound = nullptr;
	
	virtual void Die() override;

	UFUNCTION(NetMulticast,Reliable)
	void MulticastHandleDeath();

	void Dissolve();
	UFUNCTION(BlueprintImplementableEvent)
	void BeginMeshDissolveTimeline(UMaterialInstanceDynamic* MaterialInstanceDynamic);
	UFUNCTION(BlueprintImplementableEvent)
	void BeginWeaponDissolveTimeline(UMaterialInstanceDynamic* MaterialInstanceDynamic);

	bool IsDead = false;

	virtual bool IsDead_Implementation() override;

	virtual AActor* GetAvatarActor_Implementation() override;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TArray<FTaggedMontage> TaggedMontages;

	virtual TArray<FTaggedMontage> GetTaggedMontages_Implementation() override;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	UNiagaraSystem* BloodEffect;

	int FollowerNum = 0;
	
private:
	UPROPERTY(EditAnywhere,Category="GameplayAbilities")
	TArray<TSubclassOf<UGameplayAbility>> GameplayAbilities; 
	
};
