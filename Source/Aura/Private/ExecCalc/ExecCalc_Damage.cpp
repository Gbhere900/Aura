// Fill out your copyright notice in the Description page of Project Settings.


#include "ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "AuraAbilityTypes.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystermLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/CombatInterface.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"

//这里有一定要用宏定义的必要吗，其实可以手写
struct FAuraDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor)
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration)
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage)
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance)

	DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(LighteningResistance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArcaneResistance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicsResistance)

	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagToResistance;
	FAuraDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,BlockChance,Target,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,Armor,Target,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,ArmorPenetration,Source,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,CriticalHitChance,Source,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,CriticalHitDamage,Source,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,CriticalHitResistance,Target,false);

		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,FireResistance,Target,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,LighteningResistance,Target,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,ArcaneResistance,Target,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,PhysicsResistance,Target,false);

		
		TagToResistance.Add(FAuraGameplayTags::Get().Attributes_Secondary_FireResistance,FireResistanceDef);
		TagToResistance.Add(FAuraGameplayTags::Get().Attributes_Secondary_LighteningResistance,LighteningResistanceDef);
		TagToResistance.Add(FAuraGameplayTags::Get().Attributes_Secondary_ArcaneResistance,ArcaneResistanceDef);
		TagToResistance.Add(FAuraGameplayTags::Get().Attributes_Secondary_PhysicsResistance,PhysicsResistanceDef);
	}
};

//这里一定要是Static吗？难道类外部也要访问？
//为什么要用函数来获得捕获的属性？直接上面的调用捕获属性变量不行吗
static FAuraDamageStatics GetDamageStatic()
{
	//函数中定义的static变量会在第一次调用函数时创建并且保持全局的生命周期
	static FAuraDamageStatics DamageStatic;
	return DamageStatic;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	//要记得加上下面的函数
	FAuraGameplayTags::Get().InitializeNativeGameplayTags();
	RelevantAttributesToCapture.Add(GetDamageStatic().BlockChanceDef);
	RelevantAttributesToCapture.Add(GetDamageStatic().ArmorDef);
	RelevantAttributesToCapture.Add(GetDamageStatic().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(GetDamageStatic().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(GetDamageStatic().CriticalHitDamageDef);
	RelevantAttributesToCapture.Add(GetDamageStatic().CriticalHitResistanceDef);

	RelevantAttributesToCapture.Add(GetDamageStatic().FireResistanceDef);
	RelevantAttributesToCapture.Add(GetDamageStatic().LighteningResistanceDef);
	RelevantAttributesToCapture.Add(GetDamageStatic().ArcaneResistanceDef);
	RelevantAttributesToCapture.Add(GetDamageStatic().PhysicsResistanceDef);


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

	UCharacterClassInfo* CharacterClassInfo = UAuraAbilitySystermLibrary::GetCharacterClassInfo(SourceActor);
	TObjectPtr<UCurveTable> DamageCoefficientCurveTable = CharacterClassInfo->DamageCoefficientCurveTable;

	ICombatInterface* SourceCombatInterface = Cast<ICombatInterface>(SourceActor);
	ICombatInterface* TargetCombatInterface = Cast<ICombatInterface>(TargetActor);
	
	float ArmorCoefficient =0;
	ArmorCoefficient = DamageCoefficientCurveTable->FindCurve(FName("ArmorCoefficient"),FString())->Eval(TargetCombatInterface->GetLevel());
	
	float ArmorPenetrationCoefficient = 0;
	ArmorPenetrationCoefficient = DamageCoefficientCurveTable->FindCurve(FName("ArmorPenetrationCoefficient"),FString())->Eval(SourceCombatInterface->GetLevel());

	float BlockCoefficient =0;
	BlockCoefficient = DamageCoefficientCurveTable->FindCurve(FName("BlockCoefficient"),FString())->Eval(TargetCombatInterface->GetLevel());

	float CriticalHitResistanceCoefficient =0 ;
	CriticalHitResistanceCoefficient = DamageCoefficientCurveTable->FindCurve(FName("CriticalHitResistanceCoefficient"),FString())->Eval(TargetCombatInterface->GetLevel());

	float CriticalHitDamageCoefficient =0;
	CriticalHitDamageCoefficient = DamageCoefficientCurveTable->FindCurve(FName("CriticalHitDamageCoefficient"),FString())->Eval(SourceCombatInterface->GetLevel());

	//为什么Damage不用捕获智力，力量等属性获得？而是靠创建Spec时绑定的Tag到数值的映射？
	float Damage = 0;
	for (auto& pair : FAuraGameplayTags::Get().DamageTypeToResistance)
	{
		TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagToResistance = GetDamageStatic().TagToResistance;
		checkf(GetDamageStatic().TagToResistance.Contains(pair.Value),TEXT("Target"));
		float TempDamage = EffectSpec.GetSetByCallerMagnitude(pair.Key);
		float TempResistance = 0;
		FGameplayEffectAttributeCaptureDefinition DamageCapture = GetDamageStatic().TagToResistance[pair.Value];
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageCapture,EvalParams,TempResistance);

		TempDamage *= (1 - TempResistance/100);
		Damage += TempDamage;
	}
	
	
	float BlockChance = 0;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatic().BlockChanceDef,EvalParams,BlockChance);
	BlockChance = FMath::Max<float>(0,BlockChance);
	bool bIsBlocked = FMath::RandRange(1,100) <= BlockChance;
	
	FGameplayEffectContextHandle GameplayEffectContextHandle = EffectSpec.GetContext();
	UAuraAbilitySystermLibrary::SetIsBlocked(GameplayEffectContextHandle,bIsBlocked);
	Damage = bIsBlocked ? Damage * BlockCoefficient : Damage;

	float Armor  = 0;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatic().ArmorDef,EvalParams,Armor);
	Armor = FMath::Max(0,Armor);

	float ArmorPenetration = 0;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatic().ArmorPenetrationDef,EvalParams,ArmorPenetration);
	ArmorPenetration = FMath::Max(0,ArmorPenetration);

	float ValidArmor = Armor * (100 - ArmorPenetration * ArmorPenetrationCoefficient)/100;
	ValidArmor = FMath::Max(0,ValidArmor);

	Damage = Damage * (100 - ValidArmor * ArmorCoefficient)/100;

	float CriticalHitChance = 0;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatic().CriticalHitChanceDef,EvalParams,CriticalHitChance);
	CriticalHitChance = FMath::Max(0,CriticalHitChance);

	float CriticalHitDamage = 0;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatic().CriticalHitDamageDef,EvalParams,CriticalHitDamage);
	CriticalHitDamage = FMath::Max(0,CriticalHitDamage);

	float CriticalHitResistance = 0;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatic().CriticalHitResistanceDef,EvalParams,CriticalHitResistance);
	CriticalHitResistance = FMath::Max(0,CriticalHitResistance);

	float ValidCriticalHitChance = CriticalHitChance * (100 - CriticalHitResistance * CriticalHitResistanceCoefficient)/100;
	bool bIsCritical = FMath::RandRange(1,100) <= ValidCriticalHitChance;
	UAuraAbilitySystermLibrary::SetIsCriticalHit(GameplayEffectContextHandle,bIsCritical);
	Damage = bIsCritical ? Damage * (CriticalHitDamageCoefficient + CriticalHitDamage/100): Damage;
		
	FGameplayModifierEvaluatedData GameplayModifierEvaluatedData(UAuraAttributeSet::GetComingDamageAttribute(),EGameplayModOp::Override,Damage);
	OutExecutionOutput.AddOutputModifier(GameplayModifierEvaluatedData);
}
