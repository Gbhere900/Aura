// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/AbilityInfo.h"

#include "Aura/AuraLogChannels.h"

FAbilityInfo UAbilityInfo::GetAbilityInfo(const FGameplayTag& AbilityTag, const bool bLogIfNotFound)
{
	for (auto AbilityInfo : AbilityInfos)
	{
		if (AbilityInfo.AbilityTag == AbilityTag)
		{
			return AbilityInfo;
		}
	}
	if (bLogIfNotFound)
	{
		UE_LOG(LogAura, Error,TEXT("Can't find info for AbilityTag [%s] on AbilityInfo [%s]"), AbilityTag.ToString(),
		       *GetNameSafe(this));
	}
	return FAbilityInfo();
}
