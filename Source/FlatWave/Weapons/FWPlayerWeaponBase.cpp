// Fill out your copyright notice in the Description page of Project Settings.

#include "FWPlayerWeaponBase.h"
#include "FWWeaponData.h"
#include "FWPlayerCharacterBase.h"
#include "Kismet/GameplayStatics.h"
#include "FWPlayerController.h"
#include "FWProjectileData.h"

DEFINE_LOG_CATEGORY_STATIC(LogFWPlayerWeapon, Warning, All);

UFWPlayerWeaponBase::UFWPlayerWeaponBase()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void UFWPlayerWeaponBase::BeginPlay()
{
	Super::BeginPlay();

}

void UFWPlayerWeaponBase::Init(class UFWWeaponData* NewWeaponData, FVector WeaponOffset)
{
	WeaponData = NewWeaponData;
	SetRelativeLocation(WeaponOffset);
	CurrentAmmo = WeaponData->MaxAmmo;
	SetStaticMesh(WeaponData->Mesh);
}

void UFWPlayerWeaponBase::TriggerPressed()
{
	if (!WeaponData)
	{
		UE_LOG(LogFWPlayerWeapon, Warning, TEXT("No WeaponData!"));
		return;
	}
	if (CurrentAmmo <= 0)
		return;
	bTriggerPressed = true;
	if (bCanFireOnPressed && CanFire())
		FireProjectile();
}

void UFWPlayerWeaponBase::TriggerReleased()
{
	bTriggerPressed = false;
}

void UFWPlayerWeaponBase::AltTriggerPressed()
{
	if (!WeaponData)
	{
		UE_LOG(LogFWPlayerWeapon, Warning, TEXT("No WeaponData!"));
		return;
	}
	UE_LOG(LogFWPlayerWeapon, Warning, TEXT("AltTriggerClicked"));
	bAltTriggerPressed = true;
}

void UFWPlayerWeaponBase::AltTriggerReleased()
{
	UE_LOG(LogFWPlayerWeapon, Warning, TEXT("AltTriggerReleased"));
	bAltTriggerPressed = false;
}

bool UFWPlayerWeaponBase::CanFire()
{
	return CurrentAmmo > 0 && FireRateCounter <= 0.f && bTriggerPressed;
}

AFWProjectile* UFWPlayerWeaponBase::FireProjectile()
{
	UE_LOG(LogFWPlayerWeapon, Warning, TEXT("FireProjectileBase"));
	ConsumeAmmo();
	FireRateCounter = GetFireRatePerSecond();
	if (WeaponData->FireSound)
		UGameplayStatics::PlaySoundAtLocation(this, WeaponData->FireSound, GetOwner()->GetActorLocation());
	if (WeaponData->ProjectileData && WeaponData->ProjectileData->ProjectileClass)
	{
		FVector Location = GetProjectileSpawnLocation();
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
		AFWProjectile* SpawnedProjectile = GetWorld()->SpawnActor<AFWProjectile>(WeaponData->ProjectileData->ProjectileClass, Location, Rotation, SpawnParams);
		if (SpawnedProjectile)
			SpawnedProjectile->Init(WeaponData->ProjectileData);
		return SpawnedProjectile;
	}
	return nullptr;
}

void UFWPlayerWeaponBase::ChangeAmmo(int32 Amount)
{
	CurrentAmmo = FMath::Clamp(CurrentAmmo + Amount, 0, GetMaxAmmo());
}

int32 UFWPlayerWeaponBase::GetCurrentAmmo()
{
	return CurrentAmmo;
}

int32 UFWPlayerWeaponBase::GetMaxAmmo()
{
	return WeaponData->MaxAmmo;
}

void UFWPlayerWeaponBase::ConsumeAmmo()
{
	CurrentAmmo = FMath::Clamp(CurrentAmmo - WeaponData->AmmoCostPerShot, 0, GetMaxAmmo());
}

UFWWeaponData* UFWPlayerWeaponBase::GetWeaponData() const
{
	return WeaponData;
}

float UFWPlayerWeaponBase::GetFireRateCounter()
{
	return FireRateCounter;
}

float UFWPlayerWeaponBase::GetFireRatePerSecond()
{
	return 1 / (WeaponData->FireRate / 60.f);
}

void UFWPlayerWeaponBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (FireRateCounter > 0.f)
	{
		FireRateCounter -= DeltaTime;
	}
}

FVector UFWPlayerWeaponBase::GetProjectileSpawnLocation()
{
	FVector TotalOffset = WeaponData->MuzzleOffset;

	FVector ForwardOffset = GetForwardVector() * TotalOffset.X;
	FVector RightOffset = GetRightVector() * TotalOffset.Y;
	FVector UpOffset = GetUpVector() * TotalOffset.Z;
	return GetComponentLocation() + ForwardOffset + RightOffset + UpOffset;
}

class AFWPlayerCharacterBase* UFWPlayerWeaponBase::GetOwnerCharacter() const
{
	return Cast<AFWPlayerCharacterBase>(GetOwner());
}

class AFWPlayerController* UFWPlayerWeaponBase::GetOwnerPlayerController() const
{
	return Cast<AFWPlayerController>(GetOwnerCharacter()->GetController());
}
