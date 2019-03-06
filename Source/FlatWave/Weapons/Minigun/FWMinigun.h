// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FWPlayerWeapon.h"
#include "FWMinigun.generated.h"

UCLASS(Blueprintable)
class FLATWAVE_API AFWMinigun : public AFWPlayerWeapon
{
	GENERATED_BODY()
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UParticleSystemComponent* MuzzleParticles;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UParticleSystemComponent* ShellParticles;
public:
	AFWMinigun();

	void FireProjectile() override;

	void TriggerPressed() override;

		void ActivateParticles();

	bool CanStartWarmup();
	bool CanFire() override;
	UFUNCTION(BlueprintCallable, BlueprintPure)
		float GetWarmupCounter();
protected:
	void Tick(float DeltaTime) override;
	float WarmupCounter = 0.f;
};
