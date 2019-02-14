// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FWUtilities.generated.h"

UCLASS()
class FLATWAVE_API UFWUtilities : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (WorldContext = "WorldContextObject"))
		static class AFWMainGameMode* GetFWMainGameMode(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (WorldContext = "WorldContextObject"))
		static class AFWPlayerController* GetFWPlayerController(UObject* WorldContextObject, int32 Index);
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
		static void ApplyDamage(class AActor* DamagedActor, float Damage, class AController* InstigatorController, class AActor* DamageCauseer, TSubclassOf<class UDamageType> DamageType);
};
