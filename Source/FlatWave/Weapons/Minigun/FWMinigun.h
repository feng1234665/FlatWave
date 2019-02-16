// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FWPlayerWeaponBase.h"
#include "FWMinigun.generated.h"

UCLASS()
class FLATWAVE_API UFWMinigun : public UFWPlayerWeaponBase
{
	GENERATED_BODY()
public:
	void Init(class UFWWeaponData* NewWeaponData, FVector WeaponOffset) override;
	class AFWProjectile* FireProjectile() override;
protected:
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	class UParticleSystemComponent* MuzzleParticles;
};
