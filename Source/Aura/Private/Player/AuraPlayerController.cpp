// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystermLibrary.h"
#include "Interaction/EnemyInterface.h"
#include "Player/AuraEnhancedInputComponent.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates =true;

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
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
	if (GameplayTag == FAuraGameplayTags::Get().InputTag_LMB)
	{
		bTargeting = CurrentEnemy? true: false;
		bAutoRunning = false;
	}
	
	GEngine->AddOnScreenDebugMessage(1,3.f,FColor::Red,GameplayTag.ToString());
}

void AAuraPlayerController::GameplayAbilityHeldFunc(FGameplayTag GameplayTag)
{
	if (GameplayTag != FAuraGameplayTags::Get().InputTag_LMB)
	{
		if (GetAuraAbilitySystemComponent()!= nullptr)
		{
			GetAuraAbilitySystemComponent()->GameplayAbilityHeldFunc(GameplayTag);
			return ;
		}
	}

	if (bTargeting)
	{
		if (GetAuraAbilitySystemComponent())
		{
			GetAuraAbilitySystemComponent()->GameplayAbilityHeldFunc(GameplayTag);
		}
	}
	else
	{
		FollowTime += GetWorld()->GetDeltaSeconds();
		FHitResult HitResult;
		if (GetHitResultUnderCursor(ECC_Visibility,false,HitResult))
		{
			CachedDestination = HitResult.ImpactPoint;
		}

		if (APawn* ControlleredPawn = Cast<APawn>(GetPawn()))
		{
			FVector Direction = (CachedDestination -  ControlleredPawn->GetActorLocation()).GetSafeNormal();
			ControlleredPawn->AddMovementInput(Direction);
		}
	}

	GEngine->AddOnScreenDebugMessage(2,3.f,FColor::Blue,GameplayTag.ToString());
}

void AAuraPlayerController::GameplayAbilityReleasedFunc(FGameplayTag GameplayTag)
{
	if (GameplayTag != FAuraGameplayTags::Get().InputTag_LMB)
	{
		if (GetAuraAbilitySystemComponent()!= nullptr)
		{
			GetAuraAbilitySystemComponent()->GameplayAbilityRelesedFunc(GameplayTag);
			return ;
		}
	}

	if (bTargeting)
	{
		if (GetAuraAbilitySystemComponent())
		{
			GetAuraAbilitySystemComponent()->GameplayAbilityRelesedFunc(GameplayTag);
		}
	}
	else
	{
		APawn* ControlleredPawn = Cast<APawn>(GetPawn());
		if (FollowTime <= ShortestPressThreshold)
		{
			if (UNavigationPath* NavigationPath = UNavigationSystemV1::FindPathToLocationSynchronously
			(this,ControlleredPawn->GetActorLocation(),CachedDestination))
			{
				SplineComponent->ClearSplinePoints();
				for (FVector& PointLocation : NavigationPath->PathPoints)
				{
					SplineComponent->AddSplinePoint(PointLocation,ESplineCoordinateSpace::World);
					DrawDebugSphere(GetWorld(),PointLocation,10,8,FColor::Green,false,5.f);
				}
				bAutoRunning = true;
			}
		}
		FollowTime = 0.f;
		bTargeting = false;
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

