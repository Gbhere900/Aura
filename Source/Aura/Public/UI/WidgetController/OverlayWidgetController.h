// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "UI/Widget/AuraUserWidget.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "OverlayWidgetController.generated.h"

class UAbilityInfo;

USTRUCT()
struct FUIWidgetRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FGameplayTag GameplayTag;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FText Message;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TSubclassOf<UAuraUserWidget> MessageWidget;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	UTexture2D* Image = nullptr;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	UAbilityInfo* AbilityInfo;
};

struct FOnAttributeChangeData;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttirbuteChangeSignature, float, newValue);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetRowSignature,FUIWidgetRow,UIWidgetRow);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityChangeSignature,FAbilityInfo,Info);
/**
 * 
 */



UCLASS(BlueprintType, Blueprintable)
class AURA_API UOverlayWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable,Category="GAS|Attribute")
	FOnAttirbuteChangeSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable,Category="GAS|Attribute")
	FOnAttirbuteChangeSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable,Category="GAS|Attribute")
	FOnAttirbuteChangeSignature OnManaChanged;

	UPROPERTY(BlueprintAssignable,Category="GAS|Attribute")
	FOnAttirbuteChangeSignature OnMaxManaChanged;
	

	UPROPERTY(BlueprintAssignable,Category="GAS|Message")
	FMessageWidgetRowSignature MessageWidgetRowDelegate;

	virtual void BoardcastInitialAttribute() override;
	virtual void BindCallBackToDependences() override;
	
	FAbilityChangeSignature OnAbilityChangedDelegate;

protected:
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	UDataTable* DataTable;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	UAbilityInfo* AbilityInfo;
	
	template<typename T>
	
	T* GetDataTableRowByName(UDataTable* DataTable,const FGameplayTag& GameplayTag);
	void HealthChanged(const FOnAttributeChangeData& Health);
	void MaxHealthChanged(const FOnAttributeChangeData& MaxHealth);
	void ManaChanged(const FOnAttributeChangeData& Mana);
	void MaxManaChanged(const FOnAttributeChangeData& MaxMana);

	
};

template <typename T>
T* UOverlayWidgetController::GetDataTableRowByName(UDataTable* DataTable, const FGameplayTag& GameplayTag)
{
	return DataTable->FindRow<T>(GameplayTag.GetTagName(),TEXT(""));
}
