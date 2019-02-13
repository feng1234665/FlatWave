// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FWWeaponData.generated.h"

/**
 *
 */
UCLASS()
class FLATWAVE_API UFWWeaponData : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		bool bFireContinuously = true;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		float FireRate = 60.f;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		float WarmupTime = 0.f;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		int32 MaxAmmo = 100;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		class UFWProjectileData* ProjectileData;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Audio)
		class USoundBase* FireSound;
};
