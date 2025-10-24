// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GameplayAbility/AuraProjectileSpell.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/CombatInterface.h"
#include "Actor/AuraProjectile.h"
#include "Misc/LowLevelTestAdapter.h"

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	
	
}


void UAuraProjectileSpell::SpawnFireBLot(FVector TargetPosition)
{
	//让因为投射物能力只在服务器触发，并且让火球的可复制为true,客户端运行的是火球的复制
	if (!GetAvatarActorFromActorInfo()->HasAuthority())
	{
		return;
	}

	FTransform SpawnTransform = Cast<ICombatInterface>(GetAvatarActorFromActorInfo())->GetSocketTransform();
	
	AAuraProjectile* AuraProjectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(AuraProjectileClass,SpawnTransform,
		GetAvatarActorFromActorInfo(),
		Cast<APawn>(GetAvatarActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	CHECK(AuraProjectile);
	FRotator Direction =(TargetPosition - SpawnTransform.GetLocation()).Rotation();
	AuraProjectile->SetActorRotation(Direction);

	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();
	AuraProjectile->GameplayEffectSpec = SourceASC->MakeOutgoingSpec(GameplayEffect,GetAbilityLevel(),SourceASC->MakeEffectContext());
	
	AuraProjectile->FinishSpawning(SpawnTransform);
}
