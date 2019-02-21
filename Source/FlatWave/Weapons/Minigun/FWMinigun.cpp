// Fill out your copyright notice in the Description page of Project Settings.

#include "FWMinigun.h"
#include "FWMinigunData.h"
#include "FWProjectile.h"
#include "FWPlayerCharacterBase.h"
#include "FWProjectileData.h"
#include "FWPlayerController.h"
#include "FWDamgeTypeBase.h"
#include "FWUtilities.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "FlatWave.h"

void UFWMinigun::Init(UFWWeaponData* NewWeaponData, FVector WeaponOffset)
{
	Super::Init(NewWeaponData, WeaponOffset);
	if (WeaponData->MuzzleParticles)
		MuzzleParticles = UGameplayStatics::SpawnEmitterAttached(WeaponData->MuzzleParticles,
																 this,
																 NAME_None,
																 WeaponData->MuzzleOffset,
																 FRotator::ZeroRotator,
																 EAttachLocation::SnapToTarget,
																 false);
	WarmupCounter = GetWeaponDataAs<UFWMinigunData>()->WarmupTime;
	bCanFireOnPressed = false;
}

AFWProjectile* UFWMinigun::FireProjectile()
{
	AFWProjectile* SpawnedProjectile = Super::FireProjectile();
	MuzzleParticles->ActivateSystem();
	TArray<FHitResult> Hits;
	FVector Start = GetProjectileSpawnLocation();
	FVector End = Start + GetOwnerCharacter()->GetProjectileSpawnRotation().Vector() * WeaponData->ProjectileData->MaxRange;
	bool HasHit = GetWorld()->LineTraceMultiByChannel(Hits, Start, End, COLLISION_PROJECTILE);
	if (HasHit)
	{
		for (FHitResult Hit : Hits)
		{
			if (Hit.Actor.IsValid() && Hit.Actor.Get() != GetOwner())
			{
				UFWUtilities::ApplyDamage(Hit.Actor.Get(),
										  WeaponData->ProjectileData->ImpactDamage,
										  GetOwnerPlayerController(),
										  GetOwner(),
										  UFWDamgeTypeBase::StaticClass());
			}
		}
	}
	return SpawnedProjectile;
}

void UFWMinigun::TriggerPressed()
{
	Super::TriggerPressed();
	if (!bAltTriggerPressed)
	{
		FireRateCounter = GetWeaponDataAs<UFWMinigunData>()->WarmupTime;
	}
}

void UFWMinigun::TriggerReleased()
{
	Super::TriggerReleased();
}

void UFWMinigun::AltTriggerPressed()
{
	Super::AltTriggerPressed();
}

void UFWMinigun::AltTriggerReleased()
{
	Super::AltTriggerReleased();
}

bool UFWMinigun::CanStartWarmup()
{
	return bTriggerPressed || bAltTriggerPressed;
}

bool UFWMinigun::CanFire()
{
	return (bTriggerPressed || ((bTriggerPressed && bAltTriggerPressed))) && WarmupCounter >= GetWeaponDataAs<UFWMinigunData>()->WarmupTime;
}

float UFWMinigun::GetWarmupCounter()
{
	return WarmupCounter;
}

void UFWMinigun::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (FireRateCounter <= 0.f && WeaponData->bFireContinuously && CanFire())
	{
		FireProjectile();
	}
	UFWMinigunData* Data = GetWeaponDataAs<UFWMinigunData>();
	if (CanStartWarmup())
	{
		WarmupCounter = FMath::Clamp(WarmupCounter + DeltaTime, 0.f, Data->WarmupTime);
		float WarmupPercent = WarmupCounter / Data->WarmupTime;
		float RotationRate = FMath::Lerp<float>(0, Data->RotationRate, WarmupPercent);
		FRotator Rotation(0.f, 0.f, RotationRate * DeltaTime);
		AddRelativeRotation(Rotation);
	}
	else
	{
		WarmupCounter = FMath::Clamp(WarmupCounter - DeltaTime, 0.f, Data->WarmupTime);
		float WarmupPercent = WarmupCounter / Data->WarmupTime;
		float RotationRate = FMath::Lerp<float>(0, Data->RotationRate, WarmupPercent);
		FRotator Rotation(0.f, 0.f, RotationRate * DeltaTime);
		AddRelativeRotation(Rotation);
	}
}
