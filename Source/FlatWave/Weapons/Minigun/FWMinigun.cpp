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
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"

AFWMinigun::AFWMinigun()
{
	MuzzleParticles = CreateDefaultSubobject<UParticleSystemComponent>("MuzzleParticles");
	MuzzleParticles->SetupAttachment(RootComponent);
	MuzzleParticles->bAutoActivate = false;

	ShellParticles = CreateDefaultSubobject<UParticleSystemComponent>("ShellParticles");
	ShellParticles->SetupAttachment(RootComponent);
	ShellParticles->bAutoActivate = false;

	bCanFireOnPressed = false;
}

void AFWMinigun::FireProjectile()
{
	Super::FireProjectile();
	ActivateParticles();
	TArray<FHitResult> Hits;
	FVector Start = GetActorLocation();
	FVector End = Start + GetOwnerCharacter()->GetFirstPersonCameraComponent()->GetForwardVector() * WeaponData->ProjectileData->MaxRange;
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(COLLISION_PROJECTILE);
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
										  GetOwnerCharacter(),
										  UFWDamgeTypeBase::StaticClass());
			}
		}
	}
}

void AFWMinigun::TriggerPressed()
{
	Super::TriggerPressed();
	if (!bAltTriggerPressed)
	{
		FireRateCounter = GetWeaponDataAs<UFWMinigunData>()->WarmupTime;
	}
}

void AFWMinigun::ActivateParticles()
{
	ShellParticles->ActivateSystem();
	MuzzleParticles->ActivateSystem();
}

bool AFWMinigun::CanStartWarmup()
{
	return bTriggerPressed || bAltTriggerPressed;
}

bool AFWMinigun::CanFire()
{
	return (bTriggerPressed || ((bTriggerPressed && bAltTriggerPressed))) && WarmupCounter >= GetWeaponDataAs<UFWMinigunData>()->WarmupTime && CurrentAmmo > 0;
}

float AFWMinigun::GetWarmupCounter()
{
	return WarmupCounter;
}

void AFWMinigun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UFWMinigunData* Data = GetWeaponDataAs<UFWMinigunData>();
	if (FireRateCounter <= 0.f && WeaponData->bFireContinuously && CanFire())
	{
		FireProjectile();
	}
	if (CanStartWarmup())
	{
		WarmupCounter = FMath::Clamp(WarmupCounter + DeltaTime, 0.f, Data->WarmupTime);
		float WarmupPercent = WarmupCounter / Data->WarmupTime;
		float RotationRate = FMath::Lerp<float>(0, Data->RotationRate, WarmupPercent);
		FRotator Rotation(0.f, 0.f, RotationRate * DeltaTime);
		WeaponMesh->AddRelativeRotation(Rotation);
	}
	else
	{
		WarmupCounter = FMath::Clamp(WarmupCounter - DeltaTime, 0.f, Data->WarmupTime);
		float WarmupPercent = WarmupCounter / Data->WarmupTime;
		float RotationRate = FMath::Lerp<float>(0, Data->RotationRate, WarmupPercent);
		FRotator Rotation(0.f, 0.f, RotationRate * DeltaTime);
		WeaponMesh->AddRelativeRotation(Rotation);
	}
}
