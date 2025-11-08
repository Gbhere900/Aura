// Fill out your copyright notice in the Description page of Project Settings.


#include "Character//AuraCharacterBase.h"

#include "AudioMixerBlueprintLibrary.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/GameplayAbility/AuraGameplayAbility.h"
#include "Aura/Aura.h"
#include "Components/CapsuleComponent.h"
#include "Engine/SkeletalMeshSocket.h"

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

void AAuraCharacterBase::ApplyGameplayEffectSpecToSelf(const TSubclassOf<UGameplayEffect> GameplayEffect, const int EffectLevel) const
{
	check(GameplayEffect);
	FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	FGameplayEffectSpecHandle InitialGameplayEffectSpec = AbilitySystemComponent->MakeOutgoingSpec(
		GameplayEffect, EffectLevel, EffectContextHandle);
	AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*InitialGameplayEffectSpec.Data.Get(),AbilitySystemComponent);
}

FVector AAuraCharacterBase::GetSocketTransform_Implementation()
{
	check(Weapon);
	return Weapon->GetSocketLocation(SocketName);
	// FTransform SocketTransform = GetActorTransform();
	// if (Weapon->GetSocketByName(SocketName))
	// {
	// 	SocketTransform = Weapon->GetSocketByName(SocketName)->GetSocketTransform(Weapon);
	// }
	// return SocketTransform;
}


int AAuraCharacterBase::GetLevel()
{
	return 1;
}

void AAuraCharacterBase::Die()
{
	Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld,true));
	MulticastHandleDeath();
	IsDead = true;
}

void AAuraCharacterBase::Dissolve()
{
    if(IsValid(MeshMaterialInstance))
	{
		TObjectPtr<UMaterialInstanceDynamic> MeshMaterialInstanceDynamic = UMaterialInstanceDynamic::Create(MeshMaterialInstance,this);
		GetMesh()->SetMaterial(0, MeshMaterialInstanceDynamic);
		BeginMeshDissolveTimeline(MeshMaterialInstanceDynamic);
	}

	
	if(IsValid(WeaponMaterialInstance))
	{
		TObjectPtr<UMaterialInstanceDynamic> WeaponMaterialInstanceDynamic = UMaterialInstanceDynamic::Create(WeaponMaterialInstance,this);
		Weapon->SetMaterial(0, WeaponMaterialInstanceDynamic);
		BeginWeaponDissolveTimeline(WeaponMaterialInstanceDynamic);
	}
}

bool AAuraCharacterBase::IsDead_Implementation()
{
	return IsDead;
}

AActor* AAuraCharacterBase::GetAvatarActor_Implementation()
{
	return this;
}


void AAuraCharacterBase::MulticastHandleDeath_Implementation()
{
	Weapon->SetSimulatePhysics(true);
	Weapon->SetEnableGravity(true);
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	Weapon->SetCollisionResponseToAllChannels(ECR_Ignore);
	Weapon->SetCollisionResponseToChannel(ECC_WorldStatic,ECR_Block);
	
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic,ECR_Block);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Dissolve();
}






