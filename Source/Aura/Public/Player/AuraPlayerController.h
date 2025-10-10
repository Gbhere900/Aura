// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/SplineComponent.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"

class UAuraAbilitySystemComponent;
class UAuraInputConfig;
class IEnemyInterface;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AAuraPlayerController();
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void Tick(float DeltaTime) override;

	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent = nullptr;
	TObjectPtr<UAuraAbilitySystemComponent> GetAuraAbilitySystemComponent();

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	UAuraInputConfig* AuraInputConfig; 
	
private:
	UPROPERTY(EditAnywhere,Category="Input")
	TObjectPtr<UInputMappingContext> AuraContext;
	UPROPERTY(EditAnywhere,Category="Input")
	TObjectPtr<UInputAction> MoveAction;
	IEnemyInterface* LastEnemy;
	IEnemyInterface* CurrentEnemy;
	FHitResult HitResult;
	

	FVector CachedDestination = FVector::ZeroVector;
	TObjectPtr<USplineComponent> SplineComponent;
	bool bTargeting = false;
	bool bAutoRunning = false;
	float FollowTime = 0.f;
	float ShortestPressThreshold = 0.5f;
	float AutoRunAcceptanceRadius = 50.f;

	
	void Move(const FInputActionValue&  InputActionValue);
	void CursorTrace();

	void GameplayAbilityPressedFunc(FGameplayTag GameplayTag);
	void GameplayAbilityHeldFunc(FGameplayTag GameplayTag);
	void GameplayAbilityReleasedFunc(FGameplayTag GameplayTag);

	void AutoRunning();
	
	
};
