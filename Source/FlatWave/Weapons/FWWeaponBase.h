// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FWWeaponBase.generated.h"


UCLASS(ClassGroup = (Custom), Blueprintable)
class FLATWAVE_API UFWWeaponBase : public UActorComponent
{
	GENERATED_BODY()
public:
	UFWWeaponBase();

	virtual void TriggerPressed();
	virtual void TriggerReleased();

	virtual void FireProjectile();

	void AddAmmo(int32 Amount);

	int32 GetCurrentAmmo();
	int32 GetMaxAmmo();

	class UFWWeaponData* GetWeaponData() const;

protected:
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction);

	float FireRateCounter = 0.f;
	float WarmupCounter = 0.f;
	bool bStartCounters = false;

	int32 CurrentAmmo = 0;

	UPROPERTY(EditDefaultsOnly)
		class UFWWeaponData* WeaponData;

	class AFWPlayerCharacterBase* GetOwnerCharacter() const;
	class AFWPlayerController* GetOwnerPlayerController() const;
private:
	bool bTriggerPressed = false;
};
