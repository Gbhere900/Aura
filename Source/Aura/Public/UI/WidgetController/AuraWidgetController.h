// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AuraWidgetController.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;
/**
 * 
 */
 
 USTRUCT(BlueprintType)
struct  FWidgetControllerParm
 {
 	GENERATED_BODY();
 	
 	UPROPERTY(VisibleAnywhere)
 	TObjectPtr<APlayerController> PlayerController = nullptr;
 	UPROPERTY(VisibleAnywhere)
 	TObjectPtr<APlayerState> PlayerState = nullptr;
 	UPROPERTY(VisibleAnywhere)
 	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;
 	UPROPERTY(VisibleAnywhere)
 	TObjectPtr<UAttributeSet> AttributeSet = nullptr;

 	FWidgetControllerParm();
 	FWidgetControllerParm(TObjectPtr<APlayerController> PC,TObjectPtr<APlayerState> PS,TObjectPtr<UAbilitySystemComponent>ASC,TObjectPtr<UAttributeSet>AS)
 	:PlayerController(PC),PlayerState(PS),AbilitySystemComponent(ASC),AttributeSet(AS){};
 };

 
UCLASS()
class AURA_API UAuraWidgetController : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetControllerParm(FWidgetControllerParm WidgetControllerParm);

	void virtual  BoardcastInitialAttribute() const;
	void virtual BindCallBackToDependences() ;
protected:
	UPROPERTY(BlueprintReadOnly,Category="WidgetController")
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY(BlueprintReadOnly,Category="WidgetController")
	TObjectPtr<APlayerState> PlayerState;

	UPROPERTY(BlueprintReadOnly,Category="WidgetController")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly,Category="WidgetController")
	TObjectPtr<UAttributeSet> AttributeSet;
};
