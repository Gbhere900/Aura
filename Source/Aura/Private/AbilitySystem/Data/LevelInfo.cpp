// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/LevelInfo.h"

int ULevelInfo::GetLevelByXP(int XP)
{
	int Level = 0;
	for (int i = 1;i<LevelInformations.Num();i++)
	{
		if (LevelInformations[i].LevelXP <= XP)
		{
			Level = i;
		}
	}
	if (Level == 0 )
	{
		return LevelInformations.Num() - 1;
	}
	return Level;
	
}
