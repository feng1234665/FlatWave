// Fill out your copyright notice in the Description page of Project Settings.

#include "FWBTTask_ShootAtTarget.h"
#include "FWAIController.h"
#include "FWEnemyHoverTank.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UFWBTTask_ShootAtTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComponent, uint8* NodeMemory)
{
	AFWAIController* Controller = Cast<AFWAIController>(OwnerComponent.GetAIOwner());
	if (Controller)
	{
		AFWEnemyHoverTank* HoverTank = Cast<AFWEnemyHoverTank>(Controller->GetPawn());
		if (HoverTank)
		{
			HoverTank->ShootProjectile();
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}