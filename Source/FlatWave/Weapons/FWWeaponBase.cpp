// Fill out your copyright notice in the Description page of Project Settings.

#include "FWWeaponBase.h"
#include "FWWeaponData.h"
#include "FWPlayerCharacterBase.h"
#include "Kismet/GameplayStatics.h"
#include "FWPlayerController.h"
#include "FWProjectileData.h"

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

AFWProjectile* UFWWeaponBase::FireProjectile()
{
	UE_LOG(LogFWWeapon, Warning, TEXT("FireProjectileBase"));
	FireRateCounter = 1 / (WeaponData->FireRate / 60.f);
	if (WeaponData->FireSound)
		UGameplayStatics::PlaySoundAtLocation(this, WeaponData->FireSound, GetOwner()->GetActorLocation());
	if (WeaponData->ProjectileData->ProjectileClass)
	{
		FVector Location = GetOwnerCharacter()->GetProjectileSpawnLocation();
		FRotator Rotation = GetOwnerCharacter()->GetProjectileSpawnRotation();
		if (WeaponData->MaxSpread > 0.f)
		{
			FVector RotationVector = Rotation.Vector();
			RotationVector = RotationVector.RotateAngleAxis(FMath::RandRange(-WeaponData->MaxSpread, WeaponData->MaxSpread), FVector::UpVector);
			RotationVector = RotationVector.RotateAngleAxis(FMath::RandRange(-WeaponData->MaxSpread, WeaponData->MaxSpread), FVector::RightVector);
			Rotation = RotationVector.Rotation();
		}
		FActorSpawnParameters SpawnParams;
		SpawnParams.Instigator = GetOwnerCharacter();
		return GetWorld()->SpawnActor<AFWProjectile>(WeaponData->ProjectileData->ProjectileClass, Location, Rotation, SpawnParams);
	}
	return nullptr;
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
}

class AFWPlayerCharacterBase* UFWWeaponBase::GetOwnerCharacter() const
{
	return Cast<AFWPlayerCharacterBase>(GetOwner());
}

class AFWPlayerController* UFWWeaponBase::GetOwnerPlayerController() const
{
	return Cast<AFWPlayerController>(GetOwnerCharacter()->GetController());
}
