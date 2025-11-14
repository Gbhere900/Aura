// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GameplayAbility/AuraSummonGameplayAbility.h"

#include "Kismet/KismetSystemLibrary.h"

TArray<FVector> UAuraSummonGameplayAbility::GetSpawnLocations()
{
	SpawnLocations.Empty();
	AActor* AvatarActor = GetAvatarActorFromActorInfo();
	FVector SummonLocation = AvatarActor->GetActorLocation();
	FVector FaceDirection = AvatarActor->GetActorForwardVector();

	FVector LeftEdge = FaceDirection.RotateAngleAxis(-SpawnAngle/2,AvatarActor->GetActorUpVector());
	float DeltaAngle = SpawnAngle/SpawnNum;
	for (int i = 0; i < SpawnNum; i++)
	{
		FVector Direction  = LeftEdge .RotateAngleAxis( i * DeltaAngle,AvatarActor->GetActorUpVector());
		FVector Begin = SummonLocation + Direction * MinSpawnDistance;
		FVector End = SummonLocation + Direction * MaxSpawnDistance;

		float RandomValue = FMath::FRandRange(MinSpawnDistance, MaxSpawnDistance) ;
		FVector RandomLocation = SummonLocation + RandomValue * Direction;

		FHitResult Hit;
		GetWorld()->LineTraceSingleByChannel(Hit,RandomLocation + FVector(0,0,400),
			RandomLocation + FVector(0,0,-400),ECollisionChannel::ECC_WorldStatic);
		RandomLocation = Hit.ImpactPoint;
		SpawnLocations.Add(RandomLocation);
	}
	return SpawnLocations;
}
