// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FWPickupItemData.generated.h"

UENUM()
enum class EPickupType : uint8
{
	HEALTH,
	AMMO_ALL,
	AMMO_BULLET,
	AMMO_ROCKET
};


UCLASS()
class FLATWAVE_API UFWPickupItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		EPickupType Type;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		float Amount = 100.f;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		class UParticleSystem* ParticleSystem;
};
