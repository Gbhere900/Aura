// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EnemyInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEnemyInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */

//虚幻中接口的定义有些不同？以及UFUNCTION(BlueprintCallable)的标识
class AURA_API IEnemyInterface
{
	GENERATED_BODY()

public:
	virtual void HighlightEnemy() = 0;
	virtual void UnHighLightEnemy() = 0;

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	void SetTargetActor(AActor* Actor);

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	AActor* GetTargetActor();
	
protected:
};
