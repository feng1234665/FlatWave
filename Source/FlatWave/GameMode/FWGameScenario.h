// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FWEnemyCharacterBase.h"
#include "FWGameScenario.generated.h"

USTRUCT()
struct FGameWave
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly)
		TMap<TSubclassOf<AFWEnemyCharacterBase>, int32> Enemies;
};

UCLASS()
class FLATWAVE_API UFWGameScenario : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly)
		TArray<FGameWave> Waves;
};
