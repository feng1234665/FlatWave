// Fill out your copyright notice in the Description page of Project Settings.

#include "FWProjectile.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "FWProjectileData.h"
#include "FWUtilities.h"
#include "FWDamgeTypeBase.h"
#include "Kismet/GameplayStatics.h"
#include "FlatWave.h"

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
	if (ProjectileData && OtherActor != GetInstigator())
	{
		if (ProjectileData->ImpactDamage > 0.f)
		{
			UE_LOG(LogFWProjectile, Warning, TEXT("Apply Damage: %f to Actor: %s"), ProjectileData->ImpactDamage, *OtherActor->GetHumanReadableName());
			UGameplayStatics::ApplyDamage(OtherActor, ProjectileData->ImpactDamage, GetInstigatorController(), this, UFWDamgeTypeBase::StaticClass());
		}
		if (ProjectileData->bExplodeOnHit)
		{
			TArray<AActor*> IgnoredActors;
			UGameplayStatics::ApplyRadialDamage(this,
												ProjectileData->ExplosionDamage,
												GetActorLocation(),
												ProjectileData->ExplosionRange,
												UFWDamgeTypeBase::StaticClass(),
												IgnoredActors,
												this,
												GetInstigatorController(),
												ProjectileData->bScaleExplosionDamageWithDistance,
												TRACE_NOCOLLISION);
		}
		if (ProjectileData->bDestroyOnHit)
		{
			if (ProjectileData->OnDestroyParticleSystem)
				UGameplayStatics::SpawnEmitterAtLocation(this, ProjectileData->OnDestroyParticleSystem, GetActorLocation(), FRotator::ZeroRotator, true);
			Destroy();
		}
	}
}
