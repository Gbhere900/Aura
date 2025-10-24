// Fill out your copyright notice in the Description page of Project Settings.


#include "Character//AuraCharacterBase.h"

#include "AudioMixerBlueprintLibrary.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/GameplayAbility/AuraGameplayAbility.h"
#include "Aura/Aura.h"

// Sets default values
AAuraCharacterBase::AAuraCharacterBase()
{
 	
	//PrimaryActorTick.bCanEverTick = false;
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(),FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile,ECR_Overlap);
}

UAbilitySystemComponent* AAuraCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AAuraCharacterBase::AddGameplayAbilities()			//视频里是这个函数调用AuraASC中的AddGaneplayAbilities，而不是直接在这里实现
{
	if (!HasAuthority())
		return;
	for (TSubclassOf<UGameplayAbility> Ability : GameplayAbilities)
	{

		FGameplayAbilitySpec GameplayAbilitySpec = FGameplayAbilitySpec(Ability,1);
		if (const UAuraGameplayAbility* AuraGameplayAbility = Cast<UAuraGameplayAbility>(GameplayAbilitySpec.Ability))
		{
			GameplayAbilitySpec.DynamicAbilityTags.AddTag(AuraGameplayAbility->InputActionTag);
		}
			AbilitySystemComponent->GiveAbility(GameplayAbilitySpec);
	}

}

void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAuraCharacterBase::InitAbilityActorInfo()
{
}

void AAuraCharacterBase::InitialAttributeSet()
{
	ApplyGameplayEffectSpecToSelf(InitialPrimaryGamePlayEffect,1);
	ApplyGameplayEffectSpecToSelf(InitialSecondaryGamePlayEffect,1);
	ApplyGameplayEffectSpecToSelf(InitialVitalGamePlayEffect,1);
}

void AAuraCharacterBase::ApplyGameplayEffectSpecToSelf(const TSubclassOf<UGameplayEffect> GameplayEffect, const int Level) const
{
	check(GameplayEffect);
	FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	FGameplayEffectSpecHandle InitialGameplayEffectSpec = AbilitySystemComponent->MakeOutgoingSpec(
		GameplayEffect, Level, EffectContextHandle);
	AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*InitialGameplayEffectSpec.Data.Get(),AbilitySystemComponent);
}




