// Fill out your copyright notice in the Description page of Project Settings.

#include "FWWeaponBase.h"

// Sets default values for this component's properties
UFWWeaponBase::UFWWeaponBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UFWWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UFWWeaponBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

