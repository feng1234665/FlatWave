// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "FWBTTask_ShootAtTarget.generated.h"

/**
 *
 */
UCLASS()
class FLATWAVE_API UFWBTTask_ShootAtTarget : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
protected:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComponent, uint8* NodeMemory);
};
