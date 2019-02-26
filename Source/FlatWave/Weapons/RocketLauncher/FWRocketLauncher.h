// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FWPlayerWeaponBase.h"
#include "FWRocketLauncher.generated.h"

UCLASS()
class FLATWAVE_API UFWRocketLauncher : public UFWPlayerWeaponBase
{
	GENERATED_BODY()
public:

	void Init(class UFWWeaponData* WeaponData, FVector Offset);
	void UnequipWeapon() override;

	void AltTriggerPressed() override;
	void AltTriggerReleased() override;

	bool CanFire() override;

	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	class AFWProjectile* FireProjectile() override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
		int32 GetChargeAmount();
	UFUNCTION(BlueprintCallable, BlueprintPure)
		float GetChargePercent();

private:
	FVector InitialRelativeLocation;
	bool bFiringChargedRockets = false;
	int32 AmountRocketsCharged = 0;
	float ChargedRocketsFireCounter = 0.f;
	float ChargedRocketsChargeCounter = 0.f;
};
