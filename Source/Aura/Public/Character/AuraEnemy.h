// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/CombatInterface.h"
#include "Character/AuraCharacterBase.h"
#include "Components/WidgetComponent.h"
#include "Interaction/EnemyInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "AuraEnemy.generated.h"

/**
 * 
 */

UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase,public IEnemyInterface,public ICombatInterface
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

	void BoardcastInitialAttribute() ;


	void BindCallBackToDependences();

};
