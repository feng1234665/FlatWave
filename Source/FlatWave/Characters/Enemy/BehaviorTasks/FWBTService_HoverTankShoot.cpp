// Fill out your copyright notice in the Description page of Project Settings.

#include "FWBTService_HoverTankShoot.h"
#include "FWAIController.h"
#include "FWEnemyHoverTank.h"
#include "BehaviorTree/BlackboardComponent.h"

void UFWBTService_HoverTankShoot::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AFWAIController* Controller = Cast<AFWAIController>(OwnerComp.GetAIOwner());
	if (Controller)
	{
		AFWEnemyHoverTank* HoverTank = Cast<AFWEnemyHoverTank>(Controller->GetPawn());
		UObject* TargetObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(BlackboardKey.SelectedKeyName);
		AActor* Target = Cast<AActor>(TargetObject);
		if (HoverTank && Target && HoverTank->IsPointingAt(Target))
		{
			HoverTank->ShootProjectile(Target);
		}
	}
}
