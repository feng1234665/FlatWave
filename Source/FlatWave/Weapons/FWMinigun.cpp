// Fill out your copyright notice in the Description page of Project Settings.

#include "FWMinigun.h"
#include "FWWeaponData.h"
#include "FWProjectile.h"
#include "FWPlayerCharacterBase.h"
#include "FWProjectileData.h"

void UFWMinigun::FireProjectile()
{
	Super::FireProjectile();
	if (!WeaponData)
		return;
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Instigator = Cast<APawn>(GetOwner());
	GetWorld()->SpawnActor<AFWProjectile>(WeaponData->ProjectileData->ProjectileClass,
										  GetOwnerCharacter()->GetProjectileSpawnLocation(),
										  GetOwnerCharacter()->GetProjectileSpawnRotation(),
										  SpawnParameters);
}
