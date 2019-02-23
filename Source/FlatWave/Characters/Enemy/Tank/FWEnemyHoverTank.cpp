// Fill out your copyright notice in the Description page of Project Settings.

#include "FWEnemyHoverTank.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"
#include "FWPlayerCharacterBase.h"
#include "FWProjectile.h"
#include "FWProjectileData.h"

AFWEnemyHoverTank::AFWEnemyHoverTank()
{
	ChassisComponent = CreateDefaultSubobject<UStaticMeshComponent>("ChassisComponent");
	ChassisComponent->SetupAttachment(RootComponent);
	ChassisComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	TurretComponent = CreateDefaultSubobject<UStaticMeshComponent>("TurretComponent");
	TurretComponent->SetupAttachment(ChassisComponent);
	TurretComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	BarrelComponent = CreateDefaultSubobject<UStaticMeshComponent>("BarrelComponent");
	BarrelComponent->SetupAttachment(TurretComponent);
	BarrelComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	MuzzleLocationComponent = CreateDefaultSubobject<USceneComponent>("MuzzleComponent");
	MuzzleLocationComponent->SetupAttachment(BarrelComponent);
}

void AFWEnemyHoverTank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFWEnemyHoverTank::RotateTurretTowardsTarget(AActor* Target, float DeltaTime, FVector Offset /*= FVector()*/)
{
	if (!Target)
		return;
	FVector DirectionTowardsTarget = GetActorLocation() - (Target->GetActorLocation() + Offset);
	DirectionTowardsTarget.Normalize();
	FRotator CurrentRotation = TurretComponent->GetComponentRotation();
	FRotator TargetRotation = DirectionTowardsTarget.Rotation();
	TargetRotation.Roll = ChassisComponent->GetComponentRotation().Roll;
	TargetRotation.Pitch = ChassisComponent->GetComponentRotation().Pitch;
	TurretComponent->SetWorldRotation(FMath::Lerp(CurrentRotation, TargetRotation, TurretRotationSpeed * DeltaTime));
}

void AFWEnemyHoverTank::RotateBarrelTowardsTarget(AActor* Target, float DeltaTime, FVector Offset /*= FVector()*/)
{
	if (!Target)
		return;
	FVector DirectionTowardsTarget = GetActorLocation() - (Target->GetActorLocation() + Offset);
	DirectionTowardsTarget.Normalize();
	FRotator CurrentRotation = BarrelComponent->GetComponentRotation();
	FRotator TargetRotation = DirectionTowardsTarget.Rotation();
	TargetRotation.Roll = TurretComponent->GetComponentRotation().Roll;
	TargetRotation.Yaw = TurretComponent->GetComponentRotation().Yaw;
	TargetRotation.Pitch = FMath::Clamp(TargetRotation.Pitch, BarrelMinPitch, BarrelMaxPitch);
	BarrelComponent->SetWorldRotation(FMath::Lerp(CurrentRotation, TargetRotation, BarrelPitchSpeed * DeltaTime));
}

bool AFWEnemyHoverTank::IsPointingAt(AActor* Target, float Tolerance /*= 5.f*/)
{
	if (!Target)
		return false;
	FVector DirectionTowardsTarget = GetActorLocation() - Target->GetActorLocation();
	DirectionTowardsTarget.Normalize();
	FRotator CurrentRotation = BarrelComponent->GetComponentRotation();
	float PointingDifference = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(DirectionTowardsTarget, CurrentRotation.Vector())));
	return PointingDifference <= Tolerance;
}

void AFWEnemyHoverTank::ShootProjectile(AActor* TargetActor /*= nullptr*/)
{
	if (ProjectileData)
	{
		FVector MuzzleLocation = MuzzleLocationComponent->GetComponentLocation();
		FRotator Direction = MuzzleLocationComponent->GetComponentRotation();
		FActorSpawnParameters Params;
		Params.Instigator = this;
		AFWProjectile* SpawnedProjectile = GetWorld()->SpawnActor<AFWProjectile>(ProjectileData->ProjectileClass, MuzzleLocation, Direction, Params);
		if (SpawnedProjectile)
			SpawnedProjectile->Init(ProjectileData);
	}
}

bool AFWEnemyHoverTank::IsDoneFiring()
{
	return true;
}

void AFWEnemyHoverTank::OnDeath()
{
	Super::OnDeath();
	ChassisComponent->SetSimulatePhysics(true);
	ChassisComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ChassisComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	ChassisComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
}