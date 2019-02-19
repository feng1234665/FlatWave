// Fill out your copyright notice in the Description page of Project Settings.

#include "FWMainGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "FWGameScenario.h"
#include "FWEnemySpawner.h"
#include "FWUtilities.h"

AFWMainGameMode::AFWMainGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AFWMainGameMode::BeginPlay()
{
	Super::BeginPlay();
	ChooseRandomGameScenario();
	GatherEnemySpawners();
	ProcessWave(0);
}

void AFWMainGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (CurrentState == EGameState::Running)
	{
		UpdateRunning(DeltaSeconds);
	}
}

void AFWMainGameMode::UpdateRunning(float DeltaSeconds)
{
	if (IsWaveFinished(CurrentWave))
	{
		if (CurrentWave < LastWave)
		{
			++CurrentWave;
			ProcessWave(CurrentWave);
		}
		else
		{
			CurrentState = EGameState::GameOver_Win;
			UE_LOG(LogTemp, Warning, TEXT("GameWon"));
		}
	}
}

void AFWMainGameMode::SetGamePaused(bool IsPaused)
{
	if (IsPaused)
	{
		CurrentState = EGameState::Paused;
	}
	else
	{
		CurrentState = EGameState::Running;
	}
}

bool AFWMainGameMode::IsGamePaused()
{
	return CurrentState == EGameState::Paused;
}

bool AFWMainGameMode::IsGameWon()
{
	return CurrentState == EGameState::GameOver_Win;
}

bool AFWMainGameMode::IsGameLost()
{
	return CurrentState == EGameState::GameOver_Lose;
}

void AFWMainGameMode::ChooseRandomGameScenario()
{
	if (GameScenarios.Num() > 0)
	{
		CurrentGameScenario = GameScenarios[FMath::RandRange(0, GameScenarios.Num() - 1)];
		CurrentWave = 0;
		LastWave = CurrentGameScenario->Waves.Num() - 1;
	}
}

bool AFWMainGameMode::IsWaveFinished(int32 Index)
{
	for (AFWEnemySpawner* Spawner : CurrentScenarioSpawners)
	{
		if (!(Spawner->IsDoneSpawning() && Spawner->AllEnemiesKilled()))
			return false;
	}
	return true;
}

void AFWMainGameMode::ProcessWave(int32 Index)
{
	CurrentScenarioSpawners.Reset(8);
	FGameWave Wave = CurrentGameScenario->Waves[Index];
	for (const TPair<TSubclassOf<AFWEnemyCharacterBase>, int32> Enemy : Wave.Enemies)
	{
		TSubclassOf<AFWEnemyCharacterBase> EnemyClass = Enemy.Key;
		int32 Amount = Enemy.Value;
		SetupEnemySpawners(EnemyClass, Amount);
	}
}

void AFWMainGameMode::GatherEnemySpawners()
{
	TArray<AActor*> EnemySpawnerActors;
	UGameplayStatics::GetAllActorsOfClass(this, AFWEnemySpawner::StaticClass(), EnemySpawnerActors);
	for (AActor* Actor : EnemySpawnerActors)
	{
		AFWEnemySpawner* EnemySpawner = Cast<AFWEnemySpawner>(Actor);
		if (EnemySpawner)
			EnemySpawners.Add(EnemySpawner);
	}
}

void AFWMainGameMode::SetupEnemySpawners(TSubclassOf<class AFWEnemyCharacterBase> EnemyClass, int32 Amount)
{
	int32 AmountPerSpawner = Amount / EnemySpawners.Num();
	int32 SpawnsLeft = Amount;
	TArray<AFWEnemySpawner*> FittingSpawners = EnemySpawners.FilterByPredicate([EnemyClass](AFWEnemySpawner* Spawner)
	{
		return Spawner->IsSpawningClass(EnemyClass);
	});
	if (FittingSpawners.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No fitting spawners for class %s found!"), *EnemyClass->GetName());
		return;
	}
	UFWUtilities::ShuffleArray<AFWEnemySpawner*>(FittingSpawners);
	for (int32 Index = 0; Index < FittingSpawners.Num(); Index++)
	{
		AFWEnemySpawner* Spawner = FittingSpawners[Index];
		int32 ActualAmount = AmountPerSpawner;
		if (AmountPerSpawner == 0)
		{
			ActualAmount = 1;
		}
		else if (Index == FittingSpawners.Num() - 1)
		{
			// Fill last spawner
			ActualAmount = SpawnsLeft;
		}
		SpawnsLeft -= ActualAmount;
		Spawner->SetupSpawner(ActualAmount);
		CurrentScenarioSpawners.Add(Spawner);
		if (SpawnsLeft <= 0)
		{
			break;
		}
	}
}
