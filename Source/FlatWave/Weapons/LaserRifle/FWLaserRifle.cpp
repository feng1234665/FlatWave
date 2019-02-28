// Fill out your copyright notice in the Description page of Project Settings.

#include "FWLaserRifle.h"
#include "FWLaserRifleData.h"
#include "FWDamgeTypeBase.h"
#include "FWPlayerCharacterBase.h"
#include "FWPlayerController.h"
#include "FlatWave.h"
#include "FWUtilities.h"
#include "FWProjectileData.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Camera/CameraComponent.h"

void UFWLaserRifle::Init(class UFWWeaponData* NewWeaponData, FVector Offset)
{
	Super::Init(NewWeaponData, Offset);
	BeamParticles = UGameplayStatics::SpawnEmitterAttached(GetWeaponDataAs<UFWLaserRifleData>()->BeamParticles,
														   GetOwnerCharacter()->GetWeaponComponentParent(),
														   NAME_None,
														   WeaponData->MuzzleOffset,
														   FRotator::ZeroRotator,
														   EAttachLocation::Type::KeepRelativeOffset,
														   false);
	InitialRelativeLocation = GetRelativeTransform().GetLocation();
}


void UFWLaserRifle::UnequipWeapon()
{
	Super::UnequipWeapon();
	ChargeCounter = 0.f;
	SetRelativeLocation(InitialRelativeLocation);
	ChangeAmmo(ChargedAmmoCounter);
}

void UFWLaserRifle::AltTriggerReleased()
{
	Super::AltTriggerReleased();
	if (ChargedAmmoCounter > GetWeaponDataAs<UFWLaserRifleData>()->MinChargeAmount)
	{
		FireBeam();
	}
	else
	{
		ChangeAmmo(ChargedAmmoCounter);
	}
	ChargedAmmoCounter = 0;
	ChargeCounter = 0.f;
}

bool UFWLaserRifle::CanFire()
{
	return Super::CanFire() && !bAltTriggerPressed;
}

class AFWProjectile* UFWLaserRifle::FireProjectile()
{
	FVector CurrentLocalLocation = GetRelativeTransform().GetLocation();
	SetRelativeLocation(CurrentLocalLocation + FVector(-20.f, 0.f, 0.f));
	return Super::FireProjectile();
}

void UFWLaserRifle::FireBeam()
{
	UFWLaserRifleData* Data = GetWeaponDataAs<UFWLaserRifleData>();
	BeamParticles->ActivateSystem();
	TArray<FHitResult> Hits;
	FVector Start = GetOwnerCharacter()->GetWeaponComponentParent()->GetComponentLocation() + FVector(0.f, 0.f, 50.f);
	float Range = FMath::Lerp(WeaponData->ProjectileData->MaxRange, Data->MaxChargeRange, GetChargePercent());
	FVector End = Start + GetOwnerCharacter()->GetFirstPersonCameraComponent()->GetForwardVector() * Range;
	bool HasHit = GetWorld()->LineTraceMultiByChannel(Hits, Start, End, COLLISION_PROJECTILE);
	if (HasHit)
	{
		for (FHitResult Hit : Hits)
		{
			if (Hit.Actor.IsValid() && Hit.Actor.Get() != GetOwner())
			{
				int32 Damage = (int)FMath::Lerp(WeaponData->ProjectileData->ImpactDamage, Data->FullChargeDamage, GetChargePercent());
				UFWUtilities::ApplyDamage(Hit.Actor.Get(),
										  Damage,
										  GetOwnerPlayerController(),
										  GetOwner(),
										  UFWDamgeTypeBase::StaticClass());
			}
		}
	}
}

void UFWLaserRifle::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UFWLaserRifleData* Data = GetWeaponDataAs<UFWLaserRifleData>();
	if (bAltTriggerPressed)
	{
		if (ChargedAmmoCounter < Data->FullChargeAmmoCost)
		{
			ChargeCounter += DeltaTime;
			if (ChargeCounter > (Data->FullChargeTime / Data->FullChargeAmmoCost))
			{
				ChangeAmmo(-1);
				++ChargedAmmoCounter;
				ChargeCounter = 0.f;
			}
		}
	}
	else
	{
		RegenAmmoCounter += DeltaTime * Data->AmmoRechargeRate;
		if (RegenAmmoCounter > 1.f)
		{
			ChangeAmmo(1);
			RegenAmmoCounter = 0.f;
		}
	}
	if (WeaponData->bFireContinuously && CanFire())
	{
		FireProjectile();
	}
	if (FireRateCounter > 0.f)
	{
		float FireRatePercent = 1.f - (FireRateCounter / GetFireRatePerSecond());
		FVector NewRelativeLocation = FMath::Lerp<FVector>(GetRelativeTransform().GetLocation(), InitialRelativeLocation, FireRatePercent);
		SetRelativeLocation(NewRelativeLocation);
	}
}

float UFWLaserRifle::GetChargePercent()
{
	return (float)ChargedAmmoCounter / GetWeaponDataAs<UFWLaserRifleData>()->FullChargeAmmoCost;
}
