// Fill out your copyright notice in the Description page of Project Settings.

#include "FWMinigun.h"
#include "FWWeaponData.h"
#include "FWProjectile.h"
#include "FWPlayerCharacterBase.h"
#include "FWProjectileData.h"
#include "FWPlayerController.h"
#include "FWDamgeTypeBase.h"
#include "FWUtilities.h"

void UFWMinigun::FireProjectile()
{
	Super::FireProjectile();
	if (!WeaponData)
		return;
	GetOwnerPlayerController()->SpawnActor(WeaponData->ProjectileData->ProjectileClass,
										   GetOwnerCharacter()->GetProjectileSpawnLocation(),
										   GetOwnerCharacter()->GetProjectileSpawnRotation(),
										   GetOwnerCharacter());
	TArray<FHitResult> Hits;
	FCollisionObjectQueryParams Params(FCollisionObjectQueryParams::InitType::AllDynamicObjects);
	FVector Start = GetOwnerCharacter()->GetProjectileSpawnLocation();
	FVector End = Start + GetOwnerCharacter()->GetProjectileSpawnRotation().Vector() * WeaponData->ProjectileData->MaxRange;
	bool HasHit = GetWorld()->LineTraceMultiByObjectType(Hits, Start, End, Params);
	if (HasHit)
	{
		for (FHitResult Hit : Hits)
		{
			if (Hit.Actor.IsValid() && Hit.Actor.Get() != GetOwner())
			{
				GetOwnerPlayerController()->ApplyDamage(Hit.Actor.Get(),
														WeaponData->ProjectileData->ImpactDamage,
														GetOwnerPlayerController(),
														GetOwner(),
														UFWDamgeTypeBase::StaticClass());
			}
		}
	}
}
