// Fill out your copyright notice in the Description page of Project Settings.

#include "FWSpawner.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"

AFWSpawner::AFWSpawner()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	Root = CreateDefaultSubobject<USceneComponent>("RootComponent");
	RootComponent = Root;

	VisualDebugMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("VisualDebugMesh");
	VisualDebugMeshComponent->SetupAttachment(RootComponent);

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
	if (Role == ROLE_Authority)
	{
		return GetWorld()->SpawnActor<AActor>(DefaultSpawnedClass, GetActorTransform());
	}
	return nullptr;
}

