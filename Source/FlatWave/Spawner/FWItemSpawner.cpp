// Fill out your copyright notice in the Description page of Project Settings.

#include "FWItemSpawner.h"
#include "FWPlayerCharacterBase.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "FWHealthComponent.h"
#include "FWPlayerWeaponBase.h"
#include "TimerManager.h"

AFWItemSpawner::AFWItemSpawner()
{
	Root = CreateDefaultSubobject<USceneComponent>("RootComponent");
	RootComponent = Root;

	PickupCollision = CreateDefaultSubobject<USphereComponent>("CollisionComponent");
	PickupCollision->SetupAttachment(RootComponent);

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	BaseMesh->SetupAttachment(RootComponent);
	BaseMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	BoxMesh = CreateDefaultSubobject<UStaticMeshComponent>("BoxMesh");
	BoxMesh->SetupAttachment(BaseMesh);
	BoxMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AFWItemSpawner::BeginPlay()
{
	Super::BeginPlay();
	if (PickupData)
	{
		if (bSpawnOnBeginPlay)
		{
			ActivateItem();
		}
		else
		{
			GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &AFWItemSpawner::ActivateItem, SpawnInterval, false);
		}
	}
}

void AFWItemSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FRotator NewRotation = FRotator(0.f, 30.f * DeltaTime, 0.f);
	BoxMesh->AddRelativeRotation(NewRotation);
}

void AFWItemSpawner::ActivateItem()
{
	BoxMesh->SetVisibility(true);
	PickupCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AFWItemSpawner::DeactivateItem()
{
	BoxMesh->SetVisibility(false);
	PickupCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &AFWItemSpawner::ActivateItem, SpawnInterval, false);
}

bool AFWItemSpawner::CanActivateItem(class AFWPlayerCharacterBase* Player)
{
	switch (PickupData->Type)
	{
	case EPickupType::HEALTH:
		return Player->GetHealthComponent()->GetHealthPercent() < 1.f;
	case EPickupType::AMMO_ALL:
		for (TPair<EWeaponType, UFWPlayerWeaponBase*> Weapon : Player->GetWeapons())
		{
			UFWPlayerWeaponBase* PlayerWeapon = Weapon.Value;
			if (PlayerWeapon->GetCurrentAmmo() < PlayerWeapon->GetMaxAmmo())
			{
				return true;
			}
		}
		return false;
	default:
		return true;
	}
}

void AFWItemSpawner::ApplyItem(class AFWPlayerCharacterBase* Player)
{
	switch (PickupData->Type)
	{
	case EPickupType::HEALTH:
		Player->GetHealthComponent()->ChangeHealth(PickupData->Amount);
		break;
	case EPickupType::AMMO_ALL:
		for (TPair<EWeaponType, UFWPlayerWeaponBase*> Weapon : Player->GetWeapons())
		{
			Weapon.Value->ChangeAmmo(PickupData->Amount);
		}
		break;
	default:
		break;
	}
}

void AFWItemSpawner::NotifyActorBeginOverlap(AActor* OtherActor)
{
	AFWPlayerCharacterBase* Player = Cast<AFWPlayerCharacterBase>(OtherActor);
	if (Player && CanActivateItem(Player))
	{
		ApplyItem(Player);
		DeactivateItem();
	}
}
