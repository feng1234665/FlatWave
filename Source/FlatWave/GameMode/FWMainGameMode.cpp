// Fill out your copyright notice in the Description page of Project Settings.

#include "FWMainGameMode.h"
#include "FWScenarioManager.h"
#include "Kismet/GameplayStatics.h"

AFWMainGameMode::AFWMainGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AFWMainGameMode::BeginPlay()
{
	Super::BeginPlay();
	TArray<AActor*> ScenarioManagers;
	UGameplayStatics::GetAllActorsOfClass(this, AFWScenarioManager::StaticClass(), ScenarioManagers);
	if (ScenarioManagers.Num() > 0)
	{
		ScenarioManager = Cast<AFWScenarioManager>(ScenarioManagers[0]);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("NO SCENARIO MANAGER PRESENT!"));
	}
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
	if (!bEnteredGame)
	{
		OnGameStart.Broadcast();
		bEnteredGame = true;
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

void AFWMainGameMode::SetGameWon()
{
	CurrentState = EGameState::GameOver_Win;
	UE_LOG(LogTemp, Warning, TEXT("Game Won!"));
}

void AFWMainGameMode::SetGameLost()
{
	CurrentState = EGameState::GameOver_Lose;
	UE_LOG(LogTemp, Warning, TEXT("Game Lost!"));
}

class AFWScenarioManager* AFWMainGameMode::GetScenarioManager() const
{
	return ScenarioManager;
}
