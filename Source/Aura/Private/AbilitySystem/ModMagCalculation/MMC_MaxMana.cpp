// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ModMagCalculation/MMC_MaxMana.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/CombatInterface.h"

UMMC_MaxMana::UMMC_MaxMana()
{
	IntDef.AttributeToCapture = UAuraAttributeSet::GetIntelligenceAttribute();
	IntDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	IntDef.bSnapshot =false;
}

float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	float Int = 0;
	FAggregatorEvaluateParameters EvaluateParameters;
	const FGameplayTagContainer* SourceGameplayTagContainer = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetGameplayTagContainer = Spec.CapturedTargetTags.GetAggregatedTags();
	EvaluateParameters.SourceTags = SourceGameplayTagContainer;
	EvaluateParameters.TargetTags = TargetGameplayTagContainer;
	GetCapturedAttributeMagnitude(IntDef,Spec,EvaluateParameters,Int);
	int Level = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject())->GetLevel();
	return Level * 10 + 80 + Int * 5.f;
}
