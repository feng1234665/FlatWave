// Fill out your copyright notice in the Description page of Project Settings.

#include "FWBTService_RotateTurret.h"
#include "FWAIController.h"
#include "FWEnemyHoverTank.h"
#include "BehaviorTree/BlackboardComponent.h"

void UFWBTService_RotateTurret::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	AFWAIController* Controller = Cast<AFWAIController>(OwnerComp.GetAIOwner());
	if (Controller)
	{
		AFWEnemyHoverTank* HoverTank = Cast<AFWEnemyHoverTank>(Controller->GetPawn());
		if (HoverTank)
		{
			UObject* TargetObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(BlackboardKey.SelectedKeyName);
			AActor* TargetActor = Cast<AActor>(TargetObject);
			if (TargetActor)
			{
				HoverTank->RotateTurretTowardsTarget(TargetActor, DeltaSeconds);
				HoverTank->RotateBarrelTowardsTarget(TargetActor, DeltaSeconds);
			}
		}
	}
}
