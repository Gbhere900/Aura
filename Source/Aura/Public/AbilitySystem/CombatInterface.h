// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

class UNiagaraSystem;
struct FGameplayTag;

USTRUCT(BlueprintType)
struct FTaggedMontage
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* Montage = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag SocketTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag MontageTag;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USoundBase* Sound = nullptr;
	
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI,BlueprintType)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API ICombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual int GetLevel() = 0;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FVector GetSocketLocation(FGameplayTag GameplayTag);

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	UAnimMontage* GetHitReactAnimMontage();

	UFUNCTION(BlueprintCallable,BlueprintImplementableEvent)
	void FaceTarget(FVector Location);
	
	virtual void Die() = 0;

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	bool IsDead();

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	AActor* GetAvatarActor();

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	TArray<FTaggedMontage> GetTaggedMontages();

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	UNiagaraSystem* GetBloodEffect();

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	FTaggedMontage GetTaggedMontage(FGameplayTag Tag);

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	int GetFollowerNum() ;

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	void AddOrSubtractFollowerNum(int DeltaValue);
	
};

//这个类是我自己写的，不保证合理
