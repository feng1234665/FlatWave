// Fill out your copyright notice in the Description page of Project Settings.

#include "FWRocketLauncher.h"
#include "Kismet/GameplayStatics.h"
#include "FWProjectile.h"
#include "Tank/FWEnemyHoverTank.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "FWRocketLauncherData.h"

void UFWRocketLauncher::Init(class UFWWeaponData* NewWeaponData, FVector Offset)
{
	Super::Init(NewWeaponData, Offset);
	InitialRelativeLocation = GetRelativeTransform().GetLocation();
}

void UFWRocketLauncher::UnequipWeapon()
{
	Super::UnequipWeapon();
	ChargedRocketsChargeCounter = 0.f;
	AmountRocketsCharged = 0;
	bFiringChargedRockets = false;
	SetRelativeLocation(InitialRelativeLocation);
}

void UFWRocketLauncher::AltTriggerPressed()
{
	Super::AltTriggerPressed();
	ChargedRocketsChargeCounter = 0.f;
}

void UFWRocketLauncher::AltTriggerReleased()
{
	Super::AltTriggerReleased();
	if (AmountRocketsCharged > 0)
	{
		bFiringChargedRockets = true;
		ChargedRocketsFireCounter = GetWeaponDataAs<UFWRocketLauncherData>()->TimeBetweenChargedRockets;
		ChargedRocketsChargeCounter = 0.f;
	}
}

void UFWRocketLauncher::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UFWRocketLauncherData* Data = GetWeaponDataAs<UFWRocketLauncherData>();
	if (bAltTriggerPressed && AmountRocketsCharged < Data->MaxRocketsChargable)
	{
		ChargedRocketsChargeCounter += DeltaTime;
		if (ChargedRocketsChargeCounter >= Data->TimeToChargeRocket)
		{
			++AmountRocketsCharged;
			ChargedRocketsChargeCounter = 0.f;
		}
	}
	if (bFiringChargedRockets && AmountRocketsCharged > 0)
	{
		ChargedRocketsFireCounter -= DeltaTime;
		if (ChargedRocketsFireCounter <= 0.f)
		{
			FireProjectile();
			ChargedRocketsFireCounter = Data->TimeBetweenChargedRockets;
			--AmountRocketsCharged;
			if (AmountRocketsCharged == 0)
			{
				bFiringChargedRockets = false;
			}
		}
	}
	else
	{
		if (FireRateCounter > 0.f)
		{
			float FireRatePercent = 1.f - (FireRateCounter / GetFireRatePerSecond());
			FVector NewRelativeLocation = FMath::Lerp<FVector>(GetRelativeTransform().GetLocation(), InitialRelativeLocation, FireRatePercent);
			SetRelativeLocation(NewRelativeLocation);
		}
	}
}

class AFWProjectile* UFWRocketLauncher::FireProjectile()
{
	AFWProjectile* FiredRocket = Super::FireProjectile();
	FVector CurrentLocalLocation = GetRelativeTransform().GetLocation();
	SetRelativeLocation(CurrentLocalLocation + FVector(-20.f, 0.f, 0.f));
	return FiredRocket;
}

int32 UFWRocketLauncher::GetChargeAmount()
{
	return AmountRocketsCharged;
}

float UFWRocketLauncher::GetChargePercent()
{
	return ChargedRocketsChargeCounter / GetWeaponDataAs<UFWRocketLauncherData>()->TimeToChargeRocket;
}
