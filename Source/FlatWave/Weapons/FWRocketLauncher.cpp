// Fill out your copyright notice in the Description page of Project Settings.

#include "FWRocketLauncher.h"
#include "FWWeaponData.h"
#include "FWProjectileData.h"
#include "FWProjectile.h"
#include "FWPlayerCharacterBase.h"
#include "FWPlayerController.h"

void UFWRocketLauncher::FireProjectile()
{
	Super::FireProjectile();
	if (!WeaponData)
		return;
	GetOwnerPlayerController()->SpawnActor(WeaponData->ProjectileData->ProjectileClass,
										   GetOwnerCharacter()->GetProjectileSpawnLocation(),
										   GetOwnerCharacter()->GetProjectileSpawnRotation(),
										   GetOwnerCharacter());
}
