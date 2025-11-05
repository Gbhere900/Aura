// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraEnemy.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystermLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AI/AuraAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"

void AAuraEnemy::HighlightEnemy()
{
	GetMesh()->SetRenderCustomDepth(true);
}

void AAuraEnemy::UnHighLightEnemy()
{
	GetMesh()->SetRenderCustomDepth(false);
}

int AAuraEnemy::GetLevel()
{
	return Level;
}

FTransform AAuraEnemy::GetSocketTransform()
{
	FTransform SocketTransform = GetActorTransform();
	if (Weapon->GetSocketByName(SocketName))
	{
		SocketTransform = Weapon->GetSocketByName(SocketName)->GetSocketTransform(Weapon);
	}
	return SocketTransform;
}


AAuraEnemy::AAuraEnemy()
{
	//AAuraCharacterBase();
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>(FName("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	
	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>(FName("AttributeSet"));

	EnemyHealthBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(FName("EnemyHealthBar"));
	EnemyHealthBarWidgetComponent->SetupAttachment(GetRootComponent());
	//GetMesh()->SetCollisionResponseToChannel(ECC_Projectile,ECR_Block);
}

void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();
	UnHighLightEnemy();
	
	InitAbilityActorInfo();

	BindCallBackToDependences();
	BoardcastInitialAttribute();

	AbilitySystemComponent->RegisterGameplayTagEvent(FAuraGameplayTags::Get().HitReact,EGameplayTagEventType::NewOrRemoved).AddUObject(
		this,
		&AAuraEnemy::HitReactEvent);

	GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
}

void AAuraEnemy::InitAbilityActorInfo()
{
	Super::InitAbilityActorInfo();
	AbilitySystemComponent->InitAbilityActorInfo(this,this);

	InitialAttributeSet();
	InitializeGameplayAbility();
}

void AAuraEnemy::InitialAttributeSet()
{
	if (!HasAuthority())
	{
		return;
	}

	UAuraAbilitySystermLibrary::InitializeCharacterAttributeByClass(this,CharacterClass,Level,AbilitySystemComponent);
}

void AAuraEnemy::InitializeGameplayAbility()
{
	if (!HasAuthority())
	{
		return;
	}
	UAuraAbilitySystermLibrary::InitializeGameplayAbility(this,AbilitySystemComponent);
}

void AAuraEnemy::BoardcastInitialAttribute()
{
	OnHealthChangedDelegate.Broadcast(Cast<UAuraAttributeSet>(AttributeSet)->GetHealth());
	OnMaxHealthChangedDelegate.Broadcast(Cast<UAuraAttributeSet>(AttributeSet)->GetMaxHealth());
}

void AAuraEnemy::BindCallBackToDependences()
{
	UAuraUserWidget* EnemyHealthBarWidget = CastChecked<UAuraUserWidget>(EnemyHealthBarWidgetComponent->GetUserWidgetObject());
	EnemyHealthBarWidget->SetWidgetController(this);
	if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
	{
		if (UAuraAttributeSet* AuraAS = Cast<UAuraAttributeSet>(AttributeSet))
		{
			AuraASC->GetGameplayAttributeValueChangeDelegate(AuraAS->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChangedDelegate.Broadcast(Data.NewValue);
			}
			);

			AuraASC->GetGameplayAttributeValueChangeDelegate(AuraAS->GetMaxHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChangedDelegate.Broadcast(Data.NewValue);
			}
			);
		}

	}
}


//这样用HitReact Tag来设置受击状态很巧妙
void AAuraEnemy::HitReactEvent(const FGameplayTag GameplayTag, int32 Count)
{
	bool bIsHit = Count > 0;
	if (bIsHit)
	{
		GetCharacterMovement()->MaxWalkSpeed = 0;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
	}
	AuraAIController->GetBlackboardComponent()->SetValueAsBool("IsHitReact",bIsHit);
}

UAnimMontage* AAuraEnemy::GetHitReactAnimMontage_Implementation()
{
	return HitReactAnimMontage;	
}

void AAuraEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//注意只在服务端执行行为树逻辑
	if (!HasAuthority())
		return;
	AuraAIController = Cast<AAuraAIController>(NewController);
	AuraAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	AuraAIController->RunBehaviorTree(BehaviorTree);

	AuraAIController->GetBlackboardComponent()->SetValueAsBool("IsRangeAttack",CharacterClass != ECharacterClass::Warrior);
}









