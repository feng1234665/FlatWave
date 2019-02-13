// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/FWWeaponBase.h"
#include "FWRocketLauncher.generated.h"

/**
 * 
 */
UCLASS()
class FLATWAVE_API UFWRocketLauncher : public UFWWeaponBase
{
	GENERATED_BODY()
public:
	void FireProjectile();
};
