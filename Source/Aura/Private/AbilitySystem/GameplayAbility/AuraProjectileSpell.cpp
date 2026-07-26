// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GameplayAbility/AuraProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystermLibrary.h"
#include "AbilitySystem/CombatInterface.h"
#include "Actor/AuraProjectile.h"
#include "Misc/LowLevelTestAdapter.h"

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
}


void UAuraProjectileSpell::SpawnFireBLot(FVector TargetPosition,FGameplayTag SocketTag,bool bOverridePitch,float Pitch)
{
	//让因为投射物能力只在服务器触发，并且让火球的可复制为true,客户端运行的是火球的复制
	if (!GetAvatarActorFromActorInfo()->HasAuthority())
	{
		return;
	}

	FVector SpawnPosition =ICombatInterface::Execute_GetSocketLocation(GetAvatarActorFromActorInfo(),
		SocketTag);
	FTransform SpawnTransform (SpawnPosition);
	//SpawnActorDeferred是什么
	AAuraProjectile* AuraProjectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(AuraProjectileClass,SpawnTransform,
		GetAvatarActorFromActorInfo(),
		Cast<APawn>(GetAvatarActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	CHECK(AuraProjectile);
	FRotator Direction =(TargetPosition - SpawnPosition).Rotation();
	if (bOverridePitch)
	{
		Direction.Pitch = Pitch;
	}
	AuraProjectile->SetActorRotation(Direction);

	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();

	FGameplayEffectSpecHandle GameplayEffectSpecHandle = SourceASC->MakeOutgoingSpec(DamageEffect,GetAbilityLevel(),SourceASC->MakeEffectContext());
	for (auto& pair : DamageTypes)
	{
		const float DamageValue = pair.Value.GetValueAtLevel(GetAbilityLevel());
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(GameplayEffectSpecHandle,pair.Key,DamageValue);
	}

	
	AuraProjectile->GameplayEffectSpec = GameplayEffectSpecHandle;
	AuraProjectile->FinishSpawning(SpawnTransform);
}
