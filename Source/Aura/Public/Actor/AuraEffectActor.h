// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <map>

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "GameFramework/Actor.h"
#include "AuraEffectActor.generated.h"

class UAbilitySystemComponent;
class UGameplayEffect;


UENUM(BlueprintType)
enum class EEffectApplicationPolicy
{
	DoNotApply,
	ApplyOnOverlap,
	ApplyOnEndOverlap
};

UENUM(BlueprintType)
enum class EEffectRemovePolicy
{
	DoNotRemove,
	RemoveOnEndOverlap,
};



USTRUCT(BlueprintType)
struct FInstantGameplayEffectStruct
{
	GENERATED_BODY()
	
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category="AppliedEfffect")
	TSubclassOf<UGameplayEffect> GameplayEffectClass = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category="AppliedEfffect")
	EEffectApplicationPolicy GameplayEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category="AppliedEfffect")
	float Level = 1;
};

USTRUCT(BlueprintType)
struct FDurationGameplayEffectStruct
{
	GENERATED_BODY()
	
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category="AppliedEfffect")
	TSubclassOf<UGameplayEffect> GameplayEffectClass = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category="AppliedEfffect")
	EEffectApplicationPolicy GameplayEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category="AppliedEfffect")
	float Level = 1;
};

USTRUCT(BlueprintType)
struct FInfiniteGameplayEffectStruct
{
	GENERATED_BODY()
	
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category="AppliedEfffect")
	TSubclassOf<UGameplayEffect> GameplayEffectClass = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category="AppliedEfffect")
	EEffectApplicationPolicy GameplayEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category="AppliedEfffect")
	EEffectRemovePolicy GameplayEffectRemovePolicy = EEffectRemovePolicy::DoNotRemove;
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category="AppliedEfffect")
	float Level = 1;
};

UCLASS()
class AURA_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	

	AAuraEffectActor();
	UFUNCTION(BlueprintCallable)
	virtual void ApplyEffectToTarget(AActor* TargetActor,TSubclassOf<UGameplayEffect> GamePlayEffectClass
		,float Level,EEffectRemovePolicy EffectRemovePolicy);
	

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category="AppliedEfffect")
	bool bDestoryOnEffectRemoval = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category="AppliedEfffect")
	TArray<FInstantGameplayEffectStruct> InstantGameplayEffectStructs;
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category="AppliedEfffect")
	TArray<FDurationGameplayEffectStruct>	DurationGameplayEffectStructs;
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category="AppliedEfffect")
	TArray<FInfiniteGameplayEffectStruct> InfiniteGameplayEffectStructs;
	
	UFUNCTION(BlueprintCallable)
	void OnOverlap(AActor* TargetActor);
	
	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(AActor* TargetActor);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category="Destroy")
	bool bDestroyOnOverlap = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category="Destroy")
	bool bEffectEnemy = false;
	
	
	
	

private:
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> Root;

	UPROPERTY()
	TMap<FActiveGameplayEffectHandle,UAbilitySystemComponent*> ActiveEffectHandles;
	

};
