// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FWMainGameMode.generated.h"

UENUM()
enum class EGameState : uint8
{
	Running,
	Paused,
	GameOver_Win,
	GameOver_Lose
};

UCLASS()
class FLATWAVE_API AFWMainGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	AFWMainGameMode();
private:
	EGameState CurrentState;
protected:
	void BeginPlay() override;
	void Tick(float DeltaSeconds) override;
public:
	void UpdateRunning(float DeltaSeconds);

	void SetGamePaused(bool IsPaused);
	bool IsGamePaused();
	bool IsGameWon();
	bool IsGameLost();
private:
	class AFWPlayerController* PlayerControllerCache;
	class UFWGameScenario* CurrentGameScenario;
	UPROPERTY(EditDefaultsOnly)
		TArray<class UFWGameScenario*> GameScenarios;
	void ChooseRandomGameScenario();
	int32 CurrentWave = 0;
	int32 LastWave;
	bool IsWaveFinished(int32 Index);
	void ProcessWave(int32 Index);

	TArray<class AFWEnemySpawner*> EnemySpawners;
	void GatherEnemySpawners();
	TArray<class AFWEnemySpawner*> CurrentScenarioSpawners;
	void SetupEnemySpawners(TSubclassOf<class AFWEnemyCharacterBase> EnemyClass, int32 Amount);
};
