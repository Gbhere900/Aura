#pragma once


#include "GameplayEffectTypes.h"
#include "AuraAbilityTypes.generated.h"

USTRUCT(BlueprintType)
struct FAuraGameplayEffectContext:public FGameplayEffectContext
{
	GENERATED_BODY()

public:
	bool  IsCriticalHit() const {return bIsCriticalHit;}
	bool IsBlockHit() const {return bIsBlockHit;}

	void SetIsCriticalHit(bool b) {bIsCriticalHit = b;}
	void SetIsBlockHit(bool b) {bIsBlockHit = b;}
	virtual UScriptStruct* GetScriptStruct() const 
	{
		return StaticStruct();
	};

	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);

	virtual FAuraGameplayEffectContext* Duplicate() const 
	{
		FAuraGameplayEffectContext* NewContext = new FAuraGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}
	
protected:
	UPROPERTY()
	bool bIsBlockHit = false;
	UPROPERTY()
	bool bIsCriticalHit = false;
};

template<>
struct TStructOpsTypeTraits<FAuraGameplayEffectContext>:public TStructOpsTypeTraitsBase2<FAuraGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true,
	};
};
