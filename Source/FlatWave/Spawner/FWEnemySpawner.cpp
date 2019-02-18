// Fill out your copyright notice in the Description page of Project Settings.

#include "FWEnemySpawner.h"
#include "FWEnemyCharacterBase.h"

void AFWEnemySpawner::BeginPlay()
{
	bSpawnOnBeginPlay = false;
	Super::BeginPlay();
}

void AFWEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsActivated && RemainingSpawns > 0)
	{
		SpawnRateCounter += DeltaTime;
		if (SpawnRateCounter > SpawnRateSeconds)
		{
			SpawnRateCounter = 0.f;
			AActor* SpawnedActor = SpawnActorInternal(EnemyClass, GetSpawnLocation(), GetSpawnRotation());
			if (SpawnedActor)
			{
				--RemainingSpawns;
				if (bWaitForSpawnedEnemyDeaths)
				{
					SpawnedActor->OnDestroyed.AddDynamic(this, &AFWEnemySpawner::IncrementKilledEnemies);
				}
			}
		}
	}
}

void AFWEnemySpawner::SetActive(bool bIsActive)
{
	bIsActivated = bIsActive;
}

void AFWEnemySpawner::SetupSpawner(int32 Amount)
{
	RemainingSpawns = Amount;
	AmountToSpawn = Amount;
	SpawnedEnemyDeathAmount = 0;
}

bool AFWEnemySpawner::IsDoneSpawning()
{
	return RemainingSpawns == 0;
}

bool AFWEnemySpawner::AllEnemiesKilled()
{
	return !bWaitForSpawnedEnemyDeaths || SpawnedEnemyDeathAmount >= AmountToSpawn;
}

bool AFWEnemySpawner::IsSpawningClass(UClass* Class)
{
	if (!Class || !EnemyClass)
		return false;
	return EnemyClass == Class;
}

FVector AFWEnemySpawner::GetSpawnLocation()
{
	return Super::GetSpawnLocation() + SpawnOffset;
}

void AFWEnemySpawner::IncrementKilledEnemies(AActor* DestroyedActor)
{
	++SpawnedEnemyDeathAmount;
}
