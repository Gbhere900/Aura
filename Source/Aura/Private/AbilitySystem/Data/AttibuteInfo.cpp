// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/AttibuteInfo.h"

FAuraAttributeInfo UAttibuteInfo::GetArributeInfo(const FGameplayTag& GameplayTag, bool bLogNotFound)
{
	for (const FAuraAttributeInfo& AttributeInfo : AttributeInfos )		//感觉const 和 &没必要？
	{
		if (AttributeInfo.GameplayTag == GameplayTag)
		{
			return AttributeInfo;
		}
	}
	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("AttributeInfo for [%s] not found"),*GameplayTag.ToString() );
	}
	return FAuraAttributeInfo() ;
}
