// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraCharacter.h"

#include "AbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/HUD.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"

AAuraCharacter::AAuraCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0, 400.f, 0);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//Init ability actor info for the Server
	InitAbilityActorInfo();
	AddGameplayAbilities();
	
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	//Init ability actor info for the Client
	InitAbilityActorInfo();
}

int AAuraCharacter::GetLevel()
{
	return 1;
}

void AAuraCharacter::InitAbilityActorInfo()
{
	Super::InitAbilityActorInfo();
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraPlayerState,this);
	//
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	AttributeSet = AuraPlayerState->GetAttributeSet();

	//感觉下面的不用写在这个函数里面，不是这个函数的职责？写在这只是因为逻辑顺序不容易出错？
	if (AAuraPlayerController* PlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		AAuraHUD* HUD = Cast<AAuraHUD>(PlayerController->GetHUD());
		HUD->InitOverlayWidget(PlayerController,AuraPlayerState,AbilitySystemComponent,AttributeSet);
	}
	//感觉下面的不用写在这个函数里面，不是这个函数的职责？写在这只是因为逻辑顺序不容易出错(在AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent()之后)？
	//我觉得可以放在PossessedBy函数中？
	InitialAttributeSet();

}


