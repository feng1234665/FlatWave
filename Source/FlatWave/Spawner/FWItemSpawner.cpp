// Fill out your copyright notice in the Description page of Project Settings.

#include "FWItemSpawner.h"
#include "FWPlayerCharacterBase.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "FWHealthComponent.h"
#include "FWPlayerWeaponBase.h"

AFWItemSpawner::AFWItemSpawner()
{
	Root = CreateDefaultSubobject<USceneComponent>("RootComponent");
	RootComponent = Root;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>("CollisionComponent");
	CollisionComponent->SetupAttachment(RootComponent);
	ParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>("ParticleComponent");
	ParticleComponent->SetupAttachment(RootComponent);
	ParticleComponent->bAutoActivate = true;

	VisualDebugMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("DebugMesh");
	VisualDebugMeshComponent->SetupAttachment(RootComponent);
	VisualDebugMeshComponent->bHiddenInGame = true;
	VisualDebugMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AFWItemSpawner::BeginPlay()
{
	Super::BeginPlay();
	if (PickupData)
	{
		ParticleComponent->SetTemplate(PickupData->ParticleSystem);
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

void AFWItemSpawner::ActivateItem()
{
	ParticleComponent->ActivateSystem();
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AFWItemSpawner::DeactivateItem()
{
	ParticleComponent->DeactivateSystem();
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &AFWItemSpawner::ActivateItem, SpawnInterval, false);
}

bool AFWItemSpawner::CanActivateItem(class AFWPlayerCharacterBase* Player)
{
	switch (PickupData->Type)
	{
	case EPickupType::HEALTH:
		return Player->GetHealthComponent()->GetHealthPercent() < 1.f;
	case EPickupType::AMMO_ALL:
		// TODO
		return true;
	case EPickupType::AMMO_BULLET:
		// TODO
		return true;
	case EPickupType::AMMO_ROCKET:
		// TODO
		return true;
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
		// TODO
		// DEBUG
		Player->GetCurrentWeapon()->ChangeAmmo(PickupData->Amount);
		break;
	case EPickupType::AMMO_BULLET:
		// TODO
		break;
	case EPickupType::AMMO_ROCKET:
		// TODO
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
