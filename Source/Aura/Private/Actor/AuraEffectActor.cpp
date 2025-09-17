// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/AuraEffectActor.h"

#include "AbilitySystemInterface.h"
#include "AbilitySystem/AuraAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"

// Sets default values
AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
}

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, const TSubclassOf<UGameplayEffect> GamePlayEffectClass
	,float Level,EEffectRemovePolicy EffectRemovePolicy = EEffectRemovePolicy::DoNotRemove)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (TargetASC == nullptr)
	{
		return;
	}
	check(GamePlayEffectClass);
	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GamePlayEffectClass,Level,EffectContextHandle);
	FActiveGameplayEffectHandle ActiveGameplayEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(
		*EffectSpecHandle.Data.Get());
	
	//检验是否为无限效果 且会在endOverlap时移除，如果是则加入ActiveEffectHandles映射
	if (EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite
		&& EffectRemovePolicy == EEffectRemovePolicy::RemoveOnEndOverlap)
	{
		
		ActiveEffectHandles.Add(ActiveGameplayEffectHandle,TargetASC);
	}
}


void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
	
	
}

void AAuraEffectActor::OnOverlap(AActor* TargetActor)
{
	for (FInstantGameplayEffectStruct InstantGameplayEffectStruct : InstantGameplayEffectStructs)
	{
		if (InstantGameplayEffectStruct.GameplayEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
		{
			ApplyEffectToTarget(TargetActor,InstantGameplayEffectStruct.GameplayEffectClass,InstantGameplayEffectStruct.Level);
		}
	}
	for (FDurationGameplayEffectStruct DurationGameplayEffectStruct : DurationGameplayEffectStructs)
	{
		if (DurationGameplayEffectStruct.GameplayEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
		{
			ApplyEffectToTarget(TargetActor,DurationGameplayEffectStruct.GameplayEffectClass,DurationGameplayEffectStruct.Level);
		}
	}
	for (FInfiniteGameplayEffectStruct InfiniteGameplayEffectStruct : InfiniteGameplayEffectStructs)
	{
		if (InfiniteGameplayEffectStruct.GameplayEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
		{
			ApplyEffectToTarget(TargetActor,InfiniteGameplayEffectStruct.GameplayEffectClass,InfiniteGameplayEffectStruct.Level,InfiniteGameplayEffectStruct.GameplayEffectRemovePolicy);
		}
	}
	
}

void AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{
	for (FInstantGameplayEffectStruct InstantGameplayEffectStruct : InstantGameplayEffectStructs)
	{
		if (InstantGameplayEffectStruct.GameplayEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
		{
			ApplyEffectToTarget(TargetActor,InstantGameplayEffectStruct.GameplayEffectClass,InstantGameplayEffectStruct.Level);
		}
	}
	for (FDurationGameplayEffectStruct DurationGameplayEffectStruct : DurationGameplayEffectStructs)
	{
		if (DurationGameplayEffectStruct.GameplayEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
		{
			ApplyEffectToTarget(TargetActor,DurationGameplayEffectStruct.GameplayEffectClass,DurationGameplayEffectStruct.Level);
		}
	}
	for (FInfiniteGameplayEffectStruct InfiniteGameplayEffectStruct : InfiniteGameplayEffectStructs)
	{
		if (InfiniteGameplayEffectStruct.GameplayEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
		{
			ApplyEffectToTarget(TargetActor,InfiniteGameplayEffectStruct.GameplayEffectClass,InfiniteGameplayEffectStruct.Level,InfiniteGameplayEffectStruct.GameplayEffectRemovePolicy);
		}
	}

	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (TargetASC == nullptr)
	{
		return;
	}
	TArray<FActiveGameplayEffectHandle> ActiveGameplayEffectHandlesToRemove;
	for (auto Pair : ActiveEffectHandles)
	{
		if (TargetASC == Pair.Value)
		{
			ActiveGameplayEffectHandlesToRemove.Add(Pair.Key);
			TargetASC->RemoveActiveGameplayEffect(Pair.Key,1);
		}
	}
	for (FActiveGameplayEffectHandle ActiveGameplayEffectHandle : ActiveGameplayEffectHandlesToRemove)
	{
		ActiveEffectHandles.Remove(ActiveGameplayEffectHandle);
	}
		

	
}

