// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/AuraProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Aura/Aura.h"
#include "Components/AudioComponent.h"

// Sets default values
AAuraProjectile::AAuraProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;					//因为投射物能力只在服务器触发，所以让火球的可复制为true,客户端运行的是火球的复制,但是通常不建议在构造函数设置bReplicated？？？
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SetRootComponent(SphereComponent);
	SphereComponent->SetCollisionObjectType(ECC_Projectile);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComponent->SetCollisionResponseToChannel(ECC_WorldDynamic,ECR_Overlap);
	SphereComponent->SetCollisionResponseToChannel(ECC_WorldStatic,ECR_Overlap);
	SphereComponent->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this,&AAuraProjectile::OnSphereBeginOverlap);	//视频里这一句写在beginplay

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");

}

//这是根据我自己的网络理解写的
void AAuraProjectile::BeginPlay()
{
	Super::BeginPlay();
	//if(!HasAuthority())
	//{
		LoopingAudioComponent = UGameplayStatics::SpawnSoundAttached(LoopingSound,GetRootComponent());
	//}
	
	SetLifeSpan(LifeTime);
}

/*
 **下面碰撞的客户端和服务端逻辑是我凭自己理解写的
 */


void AAuraProjectile::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HasAuthority())
	{
		return;
	}
	if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
	{
		ASC->ApplyGameplayEffectSpecToSelf(*GameplayEffectSpec.Data);
	}
	Destroy();
}

void AAuraProjectile::Destroyed()
{
	Super::Destroyed();
	//if (!HasAuthority())
	//{
		LoopingAudioComponent->Stop();
		UGameplayStatics::PlaySoundAtLocation(this,HitSound,GetActorLocation(),FRotator::ZeroRotator);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this,HitParticle,GetActorLocation(),FRotator::ZeroRotator);
	//}
	
}



