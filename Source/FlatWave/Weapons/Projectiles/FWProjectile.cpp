// Fill out your copyright notice in the Description page of Project Settings.

#include "FWProjectile.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
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

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
}

void AFWProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void AFWProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFWProjectile::Init(class UFWProjectileData* NewProctileData)
{
	if (NewProctileData)
		Init(NewProctileData, FVector(NewProctileData->InitialVelocity, 0.f, 0.f));
}

void AFWProjectile::Init(class UFWProjectileData* NewProctileData, FVector CustomInitialVelocity)
{
	if (NewProctileData)
	{
		ProjectileData = NewProctileData;
		ProjectileMovement->ProjectileGravityScale = ProjectileData->GravityScale;
		ProjectileMovement->SetVelocityInLocalSpace(CustomInitialVelocity);
		SetLifeSpan(ProjectileData->Lifetime);
	}
}

class UProjectileMovementComponent* AFWProjectile::GetProjectileMovement() const
{
	return ProjectileMovement;
}

void AFWProjectile::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	if (ProjectileData && OtherActor != GetInstigator() && !Cast<AFWProjectile>(OtherActor))
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
			if (ProjectileData->OnDestroySound)
				UGameplayStatics::PlaySoundAtLocation(this, ProjectileData->OnDestroySound, GetActorLocation(), 1.f, 1.f, 0.f, ProjectileData->SoundAttenuation);
			Destroy();
		}
	}
}
