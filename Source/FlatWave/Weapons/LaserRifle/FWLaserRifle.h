// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/FWPlayerWeaponBase.h"
#include "FWLaserRifle.generated.h"

/**
 *
 */
UCLASS()
class FLATWAVE_API UFWLaserRifle : public UFWPlayerWeaponBase
{
	GENERATED_BODY()
public:

	void Init(class UFWWeaponData* WeaponData, FVector Offset);

	void UnequipWeapon() override;

	void AltTriggerReleased() override;

	bool CanFire() override;

	class AFWProjectile* FireProjectile() override;
	void FireBeam();

	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY()
		class UParticleSystemComponent* BeamParticles;
	float ChargeCounter;
	int32 ChargedAmmoCounter;
	float RegenAmmoCounter;
	UFUNCTION(BlueprintCallable, meta = (AllowPrivateAccess = "true"))
		float GetChargePercent();
	FVector InitialRelativeLocation;
};
