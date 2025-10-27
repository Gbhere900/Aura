// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/CombatInterface.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Character/AuraCharacterBase.h"
#include "Components/WidgetComponent.h"
#include "Interaction/EnemyInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "AuraEnemy.generated.h"

/**
 * 
 */

enum class ECharacterClass : uint8;

UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase,public IEnemyInterface
{
	GENERATED_BODY()
public:
	AAuraEnemy();

	/** Enemy Interface*/
	virtual void HighlightEnemy() override;
	virtual void UnHighLightEnemy() override;
	/** end Enemy Interface*/

	virtual int GetLevel() override;
	virtual FTransform GetSocketTransform() override;

	UPROPERTY(BlueprintAssignable)
	FOnAttirbuteChangeSignature OnHealthChangedDelegate;
	UPROPERTY(BlueprintAssignable)
	FOnAttirbuteChangeSignature OnMaxHealthChangedDelegate;

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	UPROPERTY(BlueprintReadOnly)
	bool bIsHighlighted = false;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Socket")
	FName SocketName = FName();

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TObjectPtr<UWidgetComponent> EnemyHealthBarWidgetComponent;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="CharacterClassDefault")
	int Level = 1;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="CharacterClassDefault")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;

	virtual void InitialAttributeSet() override;

	void InitializeGameplayAbility();

	void BoardcastInitialAttribute() ;
	
	void BindCallBackToDependences();

	void HitReactEvent(const FGameplayTag GameplayTag, int32 Count);

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	float MaxWalkSpeed = 250;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	UAnimMontage* HitReactAnimMontage;

	UFUNCTION(BlueprintCallable)
	virtual UAnimMontage* GetHitReactAnimMontage_Implementation() override;

	
};
