// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystermLibrary.h"
#include "Interaction/EnemyInterface.h"
#include "Player/AuraEnhancedInputComponent.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates =true;
}

void AAuraPlayerController::Tick(float DeltaTime)
{
	CursorTrace();
}

TObjectPtr<UAuraAbilitySystemComponent> AAuraPlayerController::GetAuraAbilitySystemComponent()
{
	
	if (AuraAbilitySystemComponent == nullptr)
	{
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>
		(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(this->GetPawn()));
	}
	return AuraAbilitySystemComponent;
}


void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (!IsLocalController())
	{
		return; // 服务器端没有本地玩家，直接返回，避免后续空值
	}
	check(AuraContext);
	check(GetLocalPlayer());
	UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	
	check(Subsystem);
	Subsystem->AddMappingContext(AuraContext,0);
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	UAuraEnhancedInputComponent* AuraEnhancedInputComponent = CastChecked<UAuraEnhancedInputComponent>(InputComponent);
	AuraEnhancedInputComponent->BindAction(MoveAction,ETriggerEvent::Triggered,this,&AAuraPlayerController::Move);
	AuraEnhancedInputComponent->BindAbilityInputAction(AuraInputConfig,this,&ThisClass::GameplayAbilityPressedFunc,&ThisClass::GameplayAbilityHeldFunc,&ThisClass::GameplayAbilityReleasedFunc);
}
void AAuraPlayerController::GameplayAbilityPressedFunc(FGameplayTag GameplayTag)
{

	GEngine->AddOnScreenDebugMessage(1,3.f,FColor::Red,GameplayTag.ToString());
}

void AAuraPlayerController::GameplayAbilityHeldFunc(FGameplayTag GameplayTag)
{
	if (GetAuraAbilitySystemComponent()!= nullptr)
	{
		GetAuraAbilitySystemComponent()->GameplayAbilityHeldFunc(GameplayTag);
	}
	GEngine->AddOnScreenDebugMessage(2,3.f,FColor::Blue,GameplayTag.ToString());
}

void AAuraPlayerController::GameplayAbilityReleasedFunc(FGameplayTag GameplayTag)
{
	if (GetAuraAbilitySystemComponent()!= nullptr)
	{
		GetAuraAbilitySystemComponent()->GameplayAbilityRelesedFunc(GameplayTag);
	}
	GEngine->AddOnScreenDebugMessage(3,3.f,FColor::Green,GameplayTag.ToString());
}



void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f,Rotation.Yaw,0.f);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection,InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection,InputAxisVector.X);
	}

}

void AAuraPlayerController::CursorTrace()
{
	FHitResult HitResult;
	GetHitResultUnderCursor(ECC_Visibility,false,HitResult);
	if (!HitResult.bBlockingHit)
		return;
	LastEnemy = CurrentEnemy;
	CurrentEnemy = Cast<IEnemyInterface>(HitResult.GetActor());
	if (CurrentEnemy == nullptr)
	{
		if (LastEnemy == nullptr)
			return;
		else
		{
			LastEnemy->UnHighLightEnemy();
		}
	}
	else
	{
		if (LastEnemy == nullptr)
		{
			CurrentEnemy->HighlightEnemy();
		}
		else
		{
			if (CurrentEnemy == LastEnemy)
			{return;}
			else
			{
				LastEnemy->UnHighLightEnemy();
				CurrentEnemy->HighlightEnemy();
			}
		}
	}
		
}

