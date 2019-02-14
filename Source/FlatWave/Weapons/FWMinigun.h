// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/FWWeaponBase.h"
#include "FWMinigun.generated.h"

/**
 * 
 */
UCLASS()
class FLATWAVE_API UFWMinigun : public UFWWeaponBase
{
	GENERATED_BODY()
public:
	class AFWProjectile* FireProjectile() override;
};
