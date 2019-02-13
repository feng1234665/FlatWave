// Fill out your copyright notice in the Description page of Project Settings.

#include "FWProjectile.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "FWProjectileData.h"
#include "FWUtilities.h"
#include "FWDamgeTypeBase.h"
#include "Net/UnrealNetwork.h"
#include "FWPlayerController.h"

DEFINE_LOG_CATEGORY_STATIC(LogFWProjectile, Warning, All);

AFWProjectile::AFWProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bReplicateMovement = true;

	Root = CreateDefaultSubobject<USceneComponent>("RootComponent");
	RootComponent = Root;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>("ProjectileMesh");
	ProjectileMesh->SetupAttachment(RootComponent);
}

void AFWProjectile::BeginPlay()
{
	Super::BeginPlay();
	if (ProjectileData)
	{
		if (ProjectileData->bSimulatePhysics)
		{
			ProjectileMesh->SetSimulatePhysics(true);
			ProjectileMesh->AddImpulse(GetActorForwardVector());
		}
		SetLifeSpan(ProjectileData->Lifetime);
	}
}

void AFWProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (ProjectileData && !ProjectileData->bSimulatePhysics)
	{
		FVector NewLocation = GetActorLocation() + GetActorForwardVector() * ProjectileData->InitialVelocity * DeltaTime;
		SetActorLocation(NewLocation, true);
	}
}

void AFWProjectile::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	if (ProjectileData && OtherActor != GetInstigator())
	{
		AFWPlayerController* PlayerController = GetPlayerInstigatorController();
		if (PlayerController)
		{
			if (ProjectileData->Type == EProjectileType::PROJECTILE)
			{
				UE_LOG(LogFWProjectile, Warning, TEXT("Apply Damage: %f to Actor: %s"), ProjectileData->ImpactDamage, *OtherActor->GetHumanReadableName());
				PlayerController->ApplyDamage(OtherActor, ProjectileData->ImpactDamage, PlayerController, this, UFWDamgeTypeBase::StaticClass());
			}
			if (ProjectileData->bDestroyOnHit)
			{
				PlayerController->DestroyActor(this);
			}
		}
	}
}

class AFWPlayerController* AFWProjectile::GetPlayerInstigatorController() const
{
	return Cast<AFWPlayerController>(GetInstigatorController());
}

void AFWProjectile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFWProjectile, ProjectileData);
}
