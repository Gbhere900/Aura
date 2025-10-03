// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/AuraHUD.h"

#include "Blueprint/UserWidget.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "UI/WidgetController/OverlayWidgetController.h"

class UAttributeSet;
class UAbilitySystemComponent;



void AAuraHUD::BeginPlay()
{
	
	
}

UOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParm& WidgetControllerParm)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this,OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParm(WidgetControllerParm);
		OverlayWidgetController->BindCallBackToDependences();
	}

	return OverlayWidgetController;
}



void AAuraHUD::InitOverlayWidget(TObjectPtr<APlayerController> PC,TObjectPtr<APlayerState> PS,TObjectPtr<UAbilitySystemComponent>ASC,TObjectPtr<UAttributeSet>AS)
{
	UUserWidget* UserWidget = CreateWidget<UUserWidget>(GetWorld(),OverlayWidgetClass);
	UAuraUserWidget* OverlayWidget = Cast<UAuraUserWidget>(UserWidget);
	FWidgetControllerParm WidgetComtrollerParm = FWidgetControllerParm(PC,PS,ASC,AS);
	GetOverlayWidgetController(WidgetComtrollerParm);
	OverlayWidget->SetWidgetController(OverlayWidgetController);
	OverlayWidgetController->BoardcastInitialAttribute();
	UserWidget->AddToViewport();
}

UAttributeMenuWidgetController* AAuraHUD::GetAttributeSetWidgetController(const FWidgetControllerParm& WidgetControllerParm)
{
	if (AttributeMenuWidgetController == nullptr)
	{
		AttributeMenuWidgetController = NewObject<UOverlayWidgetController>(this,OverlayWidgetControllerClass);
		AttributeMenuWidgetController->SetWidgetControllerParm(WidgetControllerParm);
		AttributeMenuWidgetController->BindCallBackToDependences();
	}

	return static_cast<UAttributeMenuWidgetController*>(AttributeMenuWidgetController);
}

