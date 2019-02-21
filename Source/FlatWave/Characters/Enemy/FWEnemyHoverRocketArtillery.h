// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemy/FWEnemyHoverTank.h"
#include "FWEnemyHoverRocketArtillery.generated.h"

/**
 *
 */
UCLASS()
class FLATWAVE_API AFWEnemyHoverRocketArtillery : public AFWEnemyHoverTank
{
	GENERATED_BODY()
public:
	void ShootProjectile(AActor* TargetActor = nullptr) override;
	UPROPERTY(EditDefaultsOnly)
		int32 AmountRocketsPerSalvo = 8;
	UPROPERTY(EditDefaultsOnly)
		float TimeBetweenRocketFire = .3f;
	UPROPERTY(EditDefaultsOnly)
		bool bUseHighArc = false;

	bool IsDoneFiring() override;
protected:
	void Tick(float DeltaTime) override;

	void FireRocket(AActor* Target);
	AActor* CurrentTarget;
	int32 RocketsLeftTofire = 0;
	bool bFiringRockets;
	float RocketFireCounter;
};
