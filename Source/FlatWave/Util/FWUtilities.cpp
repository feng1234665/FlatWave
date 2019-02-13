// Fill out your copyright notice in the Description page of Project Settings.

#include "FWUtilities.h"
#include "FWMainGameMode.h"
#include "FWPlayerController.h"
#include "Kismet/GameplayStatics.h"

class AFWMainGameMode* UFWUtilities::GetFWMainGameMode(UObject* WorldContextObject)
{
	return Cast<AFWMainGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
}

class AFWPlayerController* UFWUtilities::GetFWPlayerController(UObject* WorldContextObject, int32 Index)
{
	return Cast<AFWPlayerController>(UGameplayStatics::GetPlayerController(WorldContextObject, Index));
}

void UFWUtilities::ApplyDamage(class AActor* DamagedActor, float Damage, class AController* InstigatorController, class AActor* DamageCauseer, TSubclassOf<class UDamageType> DamageType)
{
	UGameplayStatics::ApplyDamage(DamagedActor, Damage, InstigatorController, DamageCauseer, DamageType);
}
