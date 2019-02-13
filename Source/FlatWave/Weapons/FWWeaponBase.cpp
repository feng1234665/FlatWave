// Fill out your copyright notice in the Description page of Project Settings.

#include "FWWeaponBase.h"
#include "FWWeaponData.h"
#include "FWPlayerCharacterBase.h"
#include "Kismet/GameplayStatics.h"
#include "FWPlayerController.h"

#include <EngineGlobals.h>
#include <Runtime/Engine/Classes/Engine/Engine.h>

DEFINE_LOG_CATEGORY_STATIC(LogFWWeapon, Warning, All);

UFWWeaponBase::UFWWeaponBase()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UFWWeaponBase::TriggerPressed()
{
	UE_LOG(LogFWWeapon, Warning, TEXT("TriggerClicked"));
	if (!WeaponData)
	{
		UE_LOG(LogFWWeapon, Warning, TEXT("No WeaponData!"));
		return;
	}
	bTriggerPressed = true;
	if (WeaponData->WarmupTime <= 0.f && FireRateCounter <= 0.f)
	{
		FireProjectile();
	}
	else
	{
		WarmupCounter = WeaponData->WarmupTime;
		FireRateCounter = WarmupCounter;
	}
}

void UFWWeaponBase::TriggerReleased()
{
	UE_LOG(LogFWWeapon, Warning, TEXT("TriggerReleased"));
	bTriggerPressed = false;
	WarmupCounter = WeaponData->WarmupTime;
}

void UFWWeaponBase::FireProjectile()
{
	UE_LOG(LogFWWeapon, Warning, TEXT("FireProjectileBase"));
	FireRateCounter = 1 / (WeaponData->FireRate / 60.f);
	UGameplayStatics::PlaySoundAtLocation(this, WeaponData->FireSound, GetOwner()->GetActorLocation());
}

void UFWWeaponBase::AddAmmo(int32 Amount)
{
	CurrentAmmo = FMath::Clamp(CurrentAmmo + Amount, 0, GetMaxAmmo());
}

int32 UFWWeaponBase::GetCurrentAmmo()
{
	return CurrentAmmo;
}

int32 UFWWeaponBase::GetMaxAmmo()
{
	return WeaponData->MaxAmmo;
}

UFWWeaponData* UFWWeaponBase::GetWeaponData() const
{
	return WeaponData;
}

void UFWWeaponBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (bTriggerPressed && WarmupCounter > 0.f)
	{
		WarmupCounter -= DeltaTime;
	}
	bool WarmupDone = WarmupCounter <= 0.f;

	if (FireRateCounter > 0.f)
	{
		FireRateCounter -= DeltaTime;
		if (WeaponData->bFireContinuously && WarmupDone && bTriggerPressed && FireRateCounter < 0.f)
		{
			FireProjectile();
		}
	}
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Red, FString::Printf(TEXT("Some variable values: x: %f, y: %f"), FireRateCounter, WarmupCounter));
}

class AFWPlayerCharacterBase* UFWWeaponBase::GetOwnerCharacter() const
{
	return Cast<AFWPlayerCharacterBase>(GetOwner());
}

class AFWPlayerController* UFWWeaponBase::GetOwnerPlayerController() const
{
	return Cast<AFWPlayerController>(GetOwnerCharacter()->GetController());
}
