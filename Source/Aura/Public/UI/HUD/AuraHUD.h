// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/Widget/AuraUserWidget.h"
#include "AuraHUD.generated.h"

class UAttributeMenuWidgetController;
class UAttributeSet;
class UAbilitySystemComponent;
class UOverlayWidgetController;
struct FWidgetControllerParm;
class UAuraWidgetController;
/**
 * 
 */
UCLASS()
class AURA_API AAuraHUD : public AHUD
{
	GENERATED_BODY()

public:
	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParm& WidgetControllerParm);
	void InitOverlayWidget(TObjectPtr<APlayerController> PC,TObjectPtr<APlayerState> PS,TObjectPtr<UAbilitySystemComponent>ASC,TObjectPtr<UAttributeSet>AS);
	
	UAttributeMenuWidgetController* GetAttributeSetWidgetController(const FWidgetControllerParm& WidgetControllerParm);
	
	
protected:
	virtual void BeginPlay() override;
	

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuraUserWidget > OverlayWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuraWidgetController> OverlayWidgetControllerClass;
	

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAuraWidgetController> AttributeMenuWidgetController;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuraWidgetController> AttributeMenuWidgetControllerClass;
};
