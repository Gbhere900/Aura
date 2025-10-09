// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController//AuraWidgetController.h"

FWidgetControllerParm::FWidgetControllerParm()
{
}

void UAuraWidgetController::SetWidgetControllerParm(FWidgetControllerParm WidgetControllerParm)
{
	PlayerController = WidgetControllerParm.PlayerController;
	PlayerState = WidgetControllerParm.PlayerState;
	AttributeSet = WidgetControllerParm.AttributeSet;
	AbilitySystemComponent = WidgetControllerParm.AbilitySystemComponent;
}

void UAuraWidgetController::BoardcastInitialAttribute() 
{
	
}

void UAuraWidgetController::BindCallBackToDependences()
{
}
