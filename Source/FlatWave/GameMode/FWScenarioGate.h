// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FWScenarioGate.generated.h"

UCLASS()
class FLATWAVE_API AFWScenarioGate : public AActor
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent)
		void Activate();
	UFUNCTION(BlueprintImplementableEvent)
		void Deactivate();
};
