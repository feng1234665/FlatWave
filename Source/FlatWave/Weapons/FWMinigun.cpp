// Fill out your copyright notice in the Description page of Project Settings.

#include "FWMinigun.h"
#include "FWWeaponData.h"
#include "FWProjectile.h"
#include "FWPlayerCharacterBase.h"
#include "FWProjectileData.h"
#include "FWPlayerController.h"

void UFWMinigun::FireProjectile()
{
	Super::FireProjectile();
	if (!WeaponData)
		return;
	GetOwnerPlayerController()->SpawnActor(WeaponData->ProjectileData->ProjectileClass,
										   GetOwnerCharacter()->GetProjectileSpawnLocation(),
										   GetOwnerCharacter()->GetProjectileSpawnRotation(),
										   GetOwnerCharacter());
}
