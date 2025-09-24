// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UI/Widget/AuraUserWidget.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "OverlayWidgetController.generated.h"

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
};

struct FOnAttributeChangeData;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, float, newHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxHealthChangedSignature,float,newMaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnManaChangedSignature, float, Mana);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxManaChangedSignature,float,MaxMana);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetRowSignature,FUIWidgetRow,UIWidgetRow);


/**
 * 
 */



UCLASS(BlueprintType, Blueprintable)
class AURA_API UOverlayWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable,Category="GAS|Attribute")
	FOnHealthChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable,Category="GAS|Attribute")
	FOnMaxHealthChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable,Category="GAS|Attribute")
	FOnHealthChangedSignature OnManaChanged;

	UPROPERTY(BlueprintAssignable,Category="GAS|Attribute")
	FOnMaxHealthChangedSignature OnMaxManaChanged;
	

	UPROPERTY(BlueprintAssignable,Category="GAS|Message")
	FMessageWidgetRowSignature MessageWidgetRowDelegate;

	virtual void BoardcastInitialAttribute() const override;
	virtual void BindCallBackToDependences() override;

protected:
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	UDataTable* DataTable;

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
