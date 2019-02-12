// Fill out your copyright notice in the Description page of Project Settings.

#include "FWSpawner.h"
#include "Components/StaticMeshComponent.h"

AFWSpawner::AFWSpawner()
{
	PrimaryActorTick.bCanEverTick = true;

	VisualDebugMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("VisualDebugMesh");
	VisualDebugMeshComponent->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AFWSpawner::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AFWSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

