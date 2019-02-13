// Fill out your copyright notice in the Description page of Project Settings.

#include "FWProjectileData.h"
#include "Net/UnrealNetwork.h"

void UFWProjectileData::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UFWProjectileData, ImpactDamage);
}

