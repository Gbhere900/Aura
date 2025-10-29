// Fill out your copyright notice in the Description page of Project Settings.


#include "ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAttributeSet.h"

//这里有一定要用宏定义的必要吗，其实可以手写
struct AuraDamageStatice
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor)
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration)
	
	AuraDamageStatice()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,BlockChance,Target,true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,Armor,Target,true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,ArmorPenetration,Source,true);
	}
};

//这里一定要是Static吗？难道类外部也要访问？
//为什么要用函数来获得捕获的属性？直接上面的调用捕获属性变量不行吗
static AuraDamageStatice GetDamageStatic()
{
	//函数中定义的static变量会在第一次调用函数时创建并且保持全局的生命周期
	static AuraDamageStatice DamageStatic;
	return DamageStatic;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	//要记得加上下面的函数
	RelevantAttributesToCapture.Add(GetDamageStatic().BlockChanceDef);
	RelevantAttributesToCapture.Add(GetDamageStatic().ArmorDef);
	RelevantAttributesToCapture.Add(GetDamageStatic().ArmorPenetrationDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceActor = SourceASC->GetAvatarActor();
	AActor* TargetActor = TargetASC->GetAvatarActor();

	
	FAggregatorEvaluateParameters EvalParams;
	FGameplayEffectSpec EffectSpec = ExecutionParams.GetOwningSpec();
	EvalParams.SourceTags = EffectSpec.CapturedSourceTags.GetAggregatedTags();
	EvalParams.TargetTags = EffectSpec.CapturedTargetTags.GetAggregatedTags();
	
	//为什么Damage不用捕获智力，力量等属性获得？而是靠创建Spec时绑定的Tag到数值的映射？
	float Damage = 0;
	Damage = EffectSpec.GetSetByCallerMagnitude(FAuraGameplayTags::Get().Damage);
	
	float BlockChance = 0;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatic().BlockChanceDef,EvalParams,BlockChance);
	BlockChance = FMath::Max<float>(0,BlockChance);
	bool bIsBlocked = FMath::RandRange(1,100) <= BlockChance;	
	Damage = bIsBlocked ? Damage/2 : Damage;

	float Armor  = 0;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatic().ArmorDef,EvalParams,Armor);
	Armor = FMath::Max(0,Armor);

	float ArmorPenetration = 0;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatic().ArmorPenetrationDef,EvalParams,ArmorPenetration);
	ArmorPenetration = FMath::Max(0,ArmorPenetration);

	float ValidArmor = Armor * (100 - ArmorPenetration)/100;
	ValidArmor = FMath::Max(0,ValidArmor);

	Damage = Damage * (100 - ValidArmor)/100;
	
	FGameplayModifierEvaluatedData GameplayModifierEvaluatedData(UAuraAttributeSet::GetComingDamageAttribute(),EGameplayModOp::Override,Damage);
	OutExecutionOutput.AddOutputModifier(GameplayModifierEvaluatedData);
}
