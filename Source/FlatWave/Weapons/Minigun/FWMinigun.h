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
	void UnequipWeapon() override;

	class AFWProjectile* FireProjectile() override;

	void TriggerPressed() override;
	void TriggerReleased() override;
	void AltTriggerPressed() override;
	void AltTriggerReleased() override;

	bool CanStartWarmup();
	bool CanFire() override;
	UFUNCTION(BlueprintCallable, BlueprintPure)
		float GetWarmupCounter();
protected:
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	float WarmupCounter = 0.f;
	class UParticleSystemComponent* MuzzleParticles;
};
