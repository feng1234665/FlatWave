// Fill out your copyright notice in the Description page of Project Settings.

#include "FWEnemyHoverRocketArtillery.h"
#include "FWProjectile.h"
#include "FWProjectileData.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

void AFWEnemyHoverRocketArtillery::ShootProjectile(AActor* TargetActor /*= nullptr*/)
{
	if (ProjectileData)
	{
		RocketsLeftTofire = AmountRocketsPerSalvo;
		CurrentTarget = TargetActor;
		RocketFireCounter = 0.f;
		bFiringRockets = true;
	}
}

bool AFWEnemyHoverRocketArtillery::IsDoneFiring()
{
	return !bFiringRockets;
}

void AFWEnemyHoverRocketArtillery::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bFiringRockets)
	{
		RocketFireCounter -= DeltaTime;
		if (RocketsLeftTofire <= 0)
		{
			bFiringRockets = false;
		}
		else if (RocketFireCounter <= 0.f)
		{
			FireRocket(CurrentTarget);
			--RocketsLeftTofire;
			RocketFireCounter = TimeBetweenRocketFire;
		}
	}
}

void AFWEnemyHoverRocketArtillery::FireRocket(AActor* Target)
{
	if (Target)
	{
		FVector Velocity;
		FVector StartLocation = MuzzleLocationComponent->GetComponentLocation();
		StartLocation += MuzzleLocationComponent->GetRightVector() * FMath::RandRange(-100.f, 100.f);
		FVector TargetLocation = Target->GetActorLocation();
		float CustomGravity = ProjectileData->GravityScale * GetWorld()->GetGravityZ();
		UGameplayStatics::SuggestProjectileVelocity_CustomArc(this, Velocity, StartLocation, TargetLocation, CustomGravity);
		if (true)
			//if (UGameplayStatics::SuggestProjectileVelocity(this, Velocity, StartLocation, TargetLocation, ProjectileData->InitialVelocity, bUseHighArc,
			//	0.f, CustomGravity, ESuggestProjVelocityTraceOption::DoNotTrace, FCollisionResponseParams::DefaultResponseParam, TArray<AActor*>(), true))
		{
			FVector MuzzleLocation = MuzzleLocationComponent->GetComponentLocation();
			Velocity.Normalize();
			FRotator Direction = Velocity.Rotation();
			FActorSpawnParameters Params;
			Params.Instigator = this;
			AFWProjectile* SpawnedProjectile = GetWorld()->SpawnActor<AFWProjectile>(ProjectileData->ProjectileClass, MuzzleLocation, Direction, Params);
			if (SpawnedProjectile)
				SpawnedProjectile->Init(ProjectileData);
		}
	}
}
