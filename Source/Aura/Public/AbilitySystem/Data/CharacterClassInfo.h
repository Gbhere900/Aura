// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterClassInfo.generated.h"

class UGameplayAbility;
class UGameplayEffect;
/**
 * 
 */
UENUM(BlueprintType)
enum class ECharacterClass  : uint8 
{
		Warrior,
		Ranger,
		Elementalist
};

USTRUCT(BlueprintType)
struct FCharacterClassDefaultInfo
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> PrimaryAttributeEffect;

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UGameplayAbility>> CharacterAbilities;
};
UCLASS()
class AURA_API UCharacterClassInfo : public UDataAsset
{
	GENERATED_BODY()


	UPROPERTY(EditDefaultsOnly)
	TMap<ECharacterClass, FCharacterClassDefaultInfo> Class_PrimaryAttributeEffectMap;

public:
	UPROPERTY(EditDefaultsOnly,Category="Common")
	TSubclassOf<UGameplayEffect> SecondaryAttributeEffect;

	UPROPERTY(EditDefaultsOnly,Category="Common")
	TSubclassOf<UGameplayEffect> VitalAttributeEffect;

	UPROPERTY(EditDefaultsOnly,Category="Common")
	TArray<TSubclassOf<UGameplayAbility>> CommonGameplayAbility;

	UPROPERTY(EditDefaultsOnly,Category="Common|Coefficient")
	TObjectPtr<UCurveTable> DamageCoefficientCurveTable;
	
	TSubclassOf<UGameplayEffect> GetCharacterClassPrimaryEffect(const ECharacterClass& CharacterClass);

	TArray<TSubclassOf<UGameplayAbility>> GetCharacterClassAbilities(const ECharacterClass& CharacterClass);
};

