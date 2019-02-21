// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "FWBTService_HoverTankShoot.generated.h"

/**
 * 
 */
UCLASS()
class FLATWAVE_API UFWBTService_HoverTankShoot : public UBTService_BlackboardBase
{
	GENERATED_BODY()
protected:
	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
