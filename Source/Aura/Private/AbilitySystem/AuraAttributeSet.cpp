// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilityTypes.h"
#include "AuraGameplayTags.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystem/AuraAbilitySystermLibrary.h"
#include "AbilitySystem/CombatInterface.h"
#include "GameFramework/Character.h"
#include "Player/AuraPlayerController.h"

FEffectProperties::FEffectProperties()
{
	
}



UAuraAttributeSet::UAuraAttributeSet()
{
	FAuraGameplayTags Tags = FAuraGameplayTags::Get();
	TagToFunctionPointer.Add({ Tags .Attributes_Primary_Strength,GetStrengthAttribute});
	TagToFunctionPointer.Add({ Tags .Attributes_Primary_Intelligence,GetIntelligenceAttribute});
	TagToFunctionPointer.Add({ Tags .Attributes_Primary_Resilience,GetResilienceAttribute});
	TagToFunctionPointer.Add({ Tags .Attributes_Primary_Vigor,GetVigorAttribute});
	TagToFunctionPointer.Add({ Tags .Attributes_Secondary_Armor,GetArmorAttribute});
	TagToFunctionPointer.Add({ Tags .Attributes_Secondary_ArmorPenetration,GetArmorAttribute});
	TagToFunctionPointer.Add({ Tags .Attributes_Secondary_BlockChance,GetBlockChanceAttribute});
	TagToFunctionPointer.Add({ Tags .Attributes_Secondary_CriticalHitChance,GetCriticalHitChanceAttribute});
	TagToFunctionPointer.Add({ Tags .Attributes_Secondary_CriticalHitDamage,GetCriticalHitDamageAttribute});
	TagToFunctionPointer.Add({ Tags .Attributes_Secondary_CriticalResistance,GetCriticalHitResistanceAttribute});
	TagToFunctionPointer.Add({ Tags .Attributes_Secondary_HealthRegeneration,GetHealthRegenerationAttribute});
	TagToFunctionPointer.Add({ Tags .Attributes_Secondary_ManaRegeneration,GetManaRegenerationAttribute});

	TagToFunctionPointer.Add({ Tags .Attributes_Secondary_FireResistance,GetFireResistanceAttribute});
	TagToFunctionPointer.Add({ Tags .Attributes_Secondary_LighteningResistance,GetLighteningResistanceAttribute});
	TagToFunctionPointer.Add({ Tags .Attributes_Secondary_ArcaneResistance,GetArcaneResistanceAttribute});
	TagToFunctionPointer.Add({ Tags .Attributes_Secondary_PhysicsResistance,GetPhysicsResistanceAttribute});
	
	TagToFunctionPointer.Add({ Tags .Attributes_Secondary_MaxHealth,GetMaxHealthAttribute});
	TagToFunctionPointer.Add({ Tags .Attributes_Secondary_MaxMana,GetMaxManaAttribute});
}

void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,Health,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,MaxHealth,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,Mana,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,MaxMana,COND_None,REPNOTIFY_Always);
}

void UAuraAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	FEffectProperties EffectProperties;
	SetEffectProperties(Data,EffectProperties);

	if (Data.EvaluatedData.Attribute == GetComingDamageAttribute())
	{
		float ComingDamageValue = GetComingDamage();
		SetComingDamage(0.f);
		if (ComingDamageValue > 0.f)
		{
			SetHealth(GetHealth() - ComingDamageValue);
			if (GetHealth() <= 0.f)
			{
				if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetActorInfo()->AvatarActor))
				{
					CombatInterface->Die();
				}
				
			}
			else
			{
				FGameplayTagContainer GameplayTagContainer;
				GameplayTagContainer.AddTag(FAuraGameplayTags::Get().HitReact);
				EffectProperties.TargetASC->TryActivateAbilitiesByTag(GameplayTagContainer);
			}

			
			//获取isCritical和设置isCritical的顺序有讲究吗
			bool bIsBlocked = UAuraAbilitySystermLibrary::GetIsBlocked(Data.EffectSpec.GetContext());
			bool bIsCriticalHit = UAuraAbilitySystermLibrary::GetIsCriticalHit(Data.EffectSpec.GetContext());
			TryShowDamageText(EffectProperties,ComingDamageValue,bIsCriticalHit,bIsBlocked);
		}
		
		
	}
	
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(),0.f,GetMaxHealth()));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
		                                 FString::Printf(
			                                 TEXT("Health changed on %s ,health = %f"),
			                                 *GetActorInfo()->AvatarActor->GetName(), GetHealth()));
		
		UE_LOG(LogTemp,Warning,TEXT("Health changed on %s ,health = %f"), *GetActorInfo()->AvatarActor->GetName(),GetHealth());
	}

	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(),0.f,GetMaxMana()));
	}
	
}

bool UAuraAttributeSet::TryShowDamageText(const FEffectProperties& EffectProperties,const float& Damage,bool bIsCriticalHit,bool bIsBlocked)
{
	if (EffectProperties.TargetAvatarActor != EffectProperties.SourceAvatarActor)
	{
		if (EffectProperties.SourceController != nullptr)
		{
			if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(EffectProperties.SourceController))
			{
				AuraPlayerController->ShowDamageText(Damage,EffectProperties.TargetAvatarActor,bIsCriticalHit,bIsBlocked);
				return true;
			}
			if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(EffectProperties.TargetController))
			{
				AuraPlayerController->ShowDamageText(Damage,EffectProperties.TargetAvatarActor,bIsCriticalHit,bIsBlocked);
				return true;
			}
		}
	}
	return false;
}


void UAuraAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

}

void UAuraAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props)
{
 	//Source = caiser of the effect,Target = target of the effect;owner if this AS
 	Props.EffectContextHandle = Data.EffectSpec.GetContext();
 	Props.SourceASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();
 	
 	if (IsValid(Props.SourceASC) && Props.SourceASC->AbilityActorInfo
 		&& Props.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
 	{
 		Props.SourceAvatarActor = Props.SourceASC->AbilityActorInfo->AvatarActor.Get();
 		Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();
 		if (Props.SourceController == nullptr && Props.SourceAvatarActor != nullptr )
 		{
 			if (const APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor))
 			{
 				Props.SourceController = Cast<APawn>(Props.SourceAvatarActor)->GetController();
 			}
 		}
 		if (Props.SourceController != nullptr)
 		{
 			
 			Props.SourceCharacter = Props.SourceController->GetCharacter();
 		}
 	}
 
 	if (Data.Target.AbilityActorInfo.IsValid()&& Data.Target.AbilityActorInfo->AvatarActor.IsValid())
 	{
 		Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
 		Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
 		Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);
 		Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
 	}
 }

void UAuraAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,Strength,OldStrength);
}

void UAuraAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,Intelligence,OldIntelligence);
}


void UAuraAttributeSet::OnRep_Resilience(const FGameplayAttributeData& OldResilience) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,Resilience,OldResilience);
}

void UAuraAttributeSet::OnRep_Vigor(const FGameplayAttributeData& OldVigor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,Vigor,OldVigor);
}

void UAuraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,MaxMana,OldMaxMana);
}

void UAuraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,MaxHealth,OldMaxHealth);
}

void UAuraAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,Armor,OldArmor);
}

void UAuraAttributeSet::OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,BlockChance,OldBlockChance);
}

void UAuraAttributeSet::OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,ArmorPenetration,OldArmorPenetration);
}

void UAuraAttributeSet::OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,CriticalHitChance,OldCriticalHitChance);
}

void UAuraAttributeSet::OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,CriticalHitDamage,OldCriticalHitDamage);
}

void UAuraAttributeSet::OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,CriticalHitResistance,OldCriticalHitResistance);
}

void UAuraAttributeSet::OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,HealthRegeneration,OldHealthRegeneration);
}

void UAuraAttributeSet::OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,ManaRegeneration,OldManaRegeneration);
}

void UAuraAttributeSet::OnRep_FireResistance(const FGameplayAttributeData& OldFireResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,FireResistance,OldFireResistance);
}

void UAuraAttributeSet::OnRep_LighteningResistance(const FGameplayAttributeData& OldLighteningResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,LighteningResistance,OldLighteningResistance);
}

void UAuraAttributeSet::OnRep_ArcaneResistance(const FGameplayAttributeData& OldArcaneResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,ArcaneResistance,OldArcaneResistance);
}

void UAuraAttributeSet::OnRep_PhysicsResistance(const FGameplayAttributeData& OldPhysicsResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,PhysicsResistance,OldPhysicsResistance);
}

void UAuraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,Health,OldHealth);
}

void UAuraAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,Mana,OldMana);
}

