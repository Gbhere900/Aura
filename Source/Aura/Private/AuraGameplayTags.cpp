// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraGameplayTags.h"
#include "GameplayTagsManager.h"
FAuraGameplayTags FAuraGameplayTags::GameplayTags;

void FAuraGameplayTags::InitializeNativeGameplayTags()
{
	/*
	 *Primary Attribute
	 **/
	GameplayTags.Attributes_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Strength"),FString("..."));
	GameplayTags.Attributes_Primary_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Intelligence"),FString("..."));
	GameplayTags.Attributes_Primary_Resilience = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Resilience"),FString("..."));
	GameplayTags.Attributes_Primary_Vigor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Vigor"),FString("..."));
	/*
	 *Secondary Attribute
	 **/
	GameplayTags.Attributes_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.MaxHealth"));
	GameplayTags.Attributes_Secondary_MaxMana = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.MaxMana"));
	GameplayTags.Attributes_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.Armor"),FString("Reduces damage Taken,improve blook chances"));
	GameplayTags.Attributes_Secondary_ArmorPenetration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.ArmorPenetration"),FString("..."));
	GameplayTags.Attributes_Secondary_BlockChance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.BlockChance"),FString("..."));
	GameplayTags.Attributes_Secondary_CriticalHitChance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.CriticalHitChance"),FString("..."));
	GameplayTags.Attributes_Secondary_CriticalHitDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.CriticalHitDamage"),FString("..."));
	GameplayTags.Attributes_Secondary_CriticalResistance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.CriticalHitResistance"),FString("..."));
	GameplayTags.Attributes_Secondary_HealthRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.HealthRegeneration"),FString("..."));
	GameplayTags.Attributes_Secondary_ManaRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.ManaRegeneration"),FString("..."));

	/*
	 *InputAction Tag
	 **/
	GameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.InputTag.LMB"));
	GameplayTags.InputTag_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.InputTag.RMB"));
	GameplayTags.InputTag_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.InputTag.1"));
	GameplayTags.InputTag_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.InputTag.2"));
	GameplayTags.InputTag_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.InputTag.3"));
	GameplayTags.InputTag_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.InputTag.4"));

	/*
	 *DamageTypes
	 */
	GameplayTags.Damage_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Fire"));;
	GameplayTags.Damage_Lightening = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Lightening"));
	GameplayTags.Damage_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Arcane"));;
	GameplayTags.Damage_Physics = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Physics"));;

	/*
	 *DamageResistance
	 */
	GameplayTags.Attributes_Secondary_FireResistance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.FireResistance"));;
	GameplayTags.Attributes_Secondary_LighteningResistance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.LighteningResistance"));;
	GameplayTags.Attributes_Secondary_ArcaneResistance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.ArcaneResistance"));;
	GameplayTags.Attributes_Secondary_PhysicsResistance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.PhysicsResistance"));;

	GameplayTags.DamageTypeToResistance.Add(GameplayTags.Damage_Fire,GameplayTags.Attributes_Secondary_FireResistance);
	GameplayTags.DamageTypeToResistance.Add(GameplayTags.Damage_Lightening,GameplayTags.Attributes_Secondary_LighteningResistance);
	GameplayTags.DamageTypeToResistance.Add(GameplayTags.Damage_Arcane,GameplayTags.Attributes_Secondary_ArcaneResistance);
	GameplayTags.DamageTypeToResistance.Add(GameplayTags.Damage_Physics,GameplayTags.Attributes_Secondary_PhysicsResistance);
	
	/*
	 *Ability
	 */
	GameplayTags.Ability_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Attack"));
	GameplayTags.Ability_Summon = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Summon"));
	/*
	 **Montage
	 */
	GameplayTags.Montage_Attack_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.1"));
	GameplayTags.Montage_Attack_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.2"));
	GameplayTags.Montage_Attack_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.3"));
	GameplayTags.Montage_Attack_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.4"));
	
	/*
	 *Socket
	 */
	GameplayTags.Socket_Weapon =  UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Socket.Weapon"));;
	GameplayTags.Socket_LeftHand = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Socket.LeftHand"));;
	GameplayTags.Socket_RightHand =  UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Socket.RightHand"));
	GameplayTags.Socket_Tail = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Socket.Tail"));
	
	/*
	 *else
	 */
	GameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage"));
	GameplayTags.HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("HitReact"));
}
