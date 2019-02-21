// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "FWBTTask_AimRocketPod.generated.h"

/**
 * 
 */
UCLASS()
class FLATWAVE_API UFWBTTask_AimRocketPod : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UFWBTTask_AimRocketPod();
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComponent, uint8* NodeMemory) override;

	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
