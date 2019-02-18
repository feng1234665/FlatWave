// Fill out your copyright notice in the Description page of Project Settings.

#include "FWSpawner.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"

AFWSpawner::AFWSpawner()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>("RootComponent");
	RootComponent = Root;

	VisualDebugMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("VisualDebugMesh");
	VisualDebugMeshComponent->SetupAttachment(RootComponent);
	VisualDebugMeshComponent->bHiddenInGame = true;
}

// Called when the game starts or when spawned
void AFWSpawner::BeginPlay()
{
	Super::BeginPlay();
	if (bSpawnOnBeginPlay)
	{
		SpawnActor();
	}
}

// Called every frame
void AFWSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

AActor* AFWSpawner::SpawnActor()
{
	return SpawnActorInternal(DefaultSpawnedClass, GetSpawnLocation(), GetSpawnRotation());
}

AActor* AFWSpawner::SpawnActorInternal(UClass* ActorClass, FVector Location, FRotator Rotation)
{
	return GetWorld()->SpawnActor<AActor>(ActorClass, Location, Rotation);
}

FVector AFWSpawner::GetSpawnLocation()
{
	return GetActorLocation();
}

FRotator AFWSpawner::GetSpawnRotation()
{
	return GetActorRotation();
}

