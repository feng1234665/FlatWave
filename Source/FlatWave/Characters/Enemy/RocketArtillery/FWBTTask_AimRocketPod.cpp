// Fill out your copyright notice in the Description page of Project Settings.

#include "FWBTTask_AimRocketPod.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "FWAIController.h"
#include "RocketArtillery/FWEnemyHoverRocketArtillery.h"

UFWBTTask_AimRocketPod::UFWBTTask_AimRocketPod()
{
	NodeName = "AimRocketPod";
	bNotifyTick = true;
}

EBTNodeResult::Type UFWBTTask_AimRocketPod::ExecuteTask(UBehaviorTreeComponent& OwnerComponent, uint8* NodeMemory)
{
	return EBTNodeResult::InProgress;
}

void UFWBTTask_AimRocketPod::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	AFWAIController* Controller = Cast<AFWAIController>(OwnerComp.GetAIOwner());
	if (Controller)
	{
		AFWEnemyHoverRocketArtillery* RocketArtillery = Cast<AFWEnemyHoverRocketArtillery>(Controller->GetPawn());
		if (RocketArtillery)
		{
			UObject* TargetObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(BlackboardKey.SelectedKeyName);
			AActor* TargetActor = Cast<AActor>(TargetObject);
			if (TargetActor)
			{
				RocketArtillery->RotateTurretTowardsTarget(TargetActor, DeltaSeconds);
				RocketArtillery->RotateBarrelTowardsTarget(TargetActor, DeltaSeconds);
				if (RocketArtillery->IsPointingAt(TargetActor))
				{
					FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				}
			}
		}
	}
}
