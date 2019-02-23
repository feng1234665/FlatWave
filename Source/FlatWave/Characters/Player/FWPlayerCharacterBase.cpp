// Fill out your copyright notice in the Description page of Project Settings.

#include "FWPlayerCharacterBase.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "FWRocketLauncher.h"
#include "FWMinigun.h"
#include "FWHealthComponent.h"
#include "FWPlayerController.h"
#include "FlatWave.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PawnMovementComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogFWPlayerCharacter, Warning, All);

AFWPlayerCharacterBase::AFWPlayerCharacterBase()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
}

void AFWPlayerCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	Weapons.RemoveAll([](UFWWeaponData* Data)
	{
		return !Data;
	});

	for (UFWWeaponData* Weapon : Weapons)
	{
		FName ComponentName(*GETENUMSTRING("EWeaponType", Weapon->Type));
		UFWPlayerWeaponBase* WeaponComponent = NewObject<UFWPlayerWeaponBase>(this, Weapon->WeaponClass, ComponentName);
		WeaponComponent->AttachToComponent(FirstPersonCameraComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
		WeaponComponent->RegisterComponent();
		WeaponComponent->SetVisibility(false);
		WeaponComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		WeaponComponent->Init(Weapon, WeaponOffset);
		WeaponComponents.Add(Weapon->Type, WeaponComponent);
	}
	if (WeaponComponents.Num() > 0)
	{
		CurrentWeapon = WeaponComponents[Weapons[0]->Type];
		CurrentWeapon->SetVisibility(true);
	}
}

void AFWPlayerCharacterBase::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	bCanDodge = true;
}

void AFWPlayerCharacterBase::EquipWeapon(int32 Index)
{
	if (CurrentWeapon && CurrentWeapon->GetWeaponData() != Weapons[Index])
	{
		CurrentWeapon->SetVisibility(false);
		CurrentWeapon->UnequipWeapon();
	}
	CurrentWeapon = WeaponComponents[Weapons[Index]->Type];
	CurrentWeapon->EquipWeapon();
	CurrentWeapon->SetVisibility(true);
}

float AFWPlayerCharacterBase::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	if (!Cast<AFWPlayerController>(EventInstigator))
	{
		return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	}
	return 0.f;
}

void AFWPlayerCharacterBase::OnDeath()
{
	DisableInput(Cast<APlayerController>(GetController()));
}

class UFWPlayerWeaponBase* AFWPlayerCharacterBase::GetCurrentWeapon() const
{
	return CurrentWeapon;
}

FRotator AFWPlayerCharacterBase::GetProjectileSpawnRotation()
{
	return GetControlRotation();
}

void AFWPlayerCharacterBase::OnTriggerPressed()
{
	if (CurrentWeapon)
		CurrentWeapon->TriggerPressed();
}

void AFWPlayerCharacterBase::OnTriggerReleased()
{
	if (CurrentWeapon)
		CurrentWeapon->TriggerReleased();
}

void AFWPlayerCharacterBase::OnAltTriggerPressed()
{
	if (CurrentWeapon)
		CurrentWeapon->AltTriggerPressed();
}

void AFWPlayerCharacterBase::OnAltTriggerReleased()
{
	if (CurrentWeapon)
		CurrentWeapon->AltTriggerReleased();
}

void AFWPlayerCharacterBase::SwitchToFirstWeapon()
{
	EquipWeapon(0);
}

void AFWPlayerCharacterBase::SwitchToSecondWeapon()
{
	EquipWeapon(1);
}

void AFWPlayerCharacterBase::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AFWPlayerCharacterBase::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AFWPlayerCharacterBase::JumpPressed()
{
	if (CanJump())
	{
		if (JumpSound)
			UGameplayStatics::PlaySoundAtLocation(this, JumpSound, GetActorLocation());
		Jump();
	}
}

void AFWPlayerCharacterBase::JumpReleased()
{
	StopJumping();
}

void AFWPlayerCharacterBase::DodgePressed()
{
	if (bCanDodge)
	{
		FVector CurrentMoveDirection = GetMovementComponent()->GetLastInputVector();
		if (CurrentMoveDirection.Size() < 0.1f)
			return;
		bCanDodge = false;
		CurrentMoveDirection.Z = DodgeVerticalPart;
		CurrentMoveDirection.Normalize();
		FVector LaunchVelocity = CurrentMoveDirection * DodgeVelocity;
		LaunchCharacter(LaunchVelocity, true, true);
	}
}
