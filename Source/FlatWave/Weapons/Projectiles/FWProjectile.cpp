// Fill out your copyright notice in the Description page of Project Settings.

#include "FWProjectile.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "FWProjectileData.h"
#include "FWUtilities.h"
#include "FWDamgeTypeBase.h"

DEFINE_LOG_CATEGORY_STATIC(LogFWProjectile, Warning, All);

AFWProjectile::AFWProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

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
	if (ProjectileData && OtherActor->GetInstigator() != GetInstigator())
	{
		UE_LOG(LogFWProjectile, Warning, TEXT("Actor overlapped: %s"), *OtherActor->GetHumanReadableName());
		UFWUtilities::ApplyDamage(OtherActor, ProjectileData->ImpactDamage, GetInstigatorController(), this, UFWDamgeTypeBase::StaticClass());
		Destroy();
	}
}

