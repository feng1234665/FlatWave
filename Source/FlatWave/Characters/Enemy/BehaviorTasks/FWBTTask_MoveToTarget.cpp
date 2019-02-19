// Fill out your copyright notice in the Description page of Project Settings.

#include "FWBTTask_MoveToTarget.h"
#include "FWAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"


EBTNodeResult::Type UFWBTTask_MoveToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComponent, uint8* NodeMemory)
{
	AFWAIController* Controller = Cast<AFWAIController>(OwnerComponent.GetAIOwner());
	if (!Controller)
	{
		return EBTNodeResult::Failed;
	}

	UObject* TargetObject = OwnerComponent.GetBlackboardComponent()->GetValueAsObject(BlackboardKey.SelectedKeyName);
	if (TargetObject)
	{
		AActor* TargetActor = Cast<AActor>(TargetObject);
		if (TargetActor)
		{
			Controller->MoveToActor(TargetActor, AcceptanceRadius, true, true, bCanStrafe);
			if (bResetBlackboardKeyOnReached)
				Controller->ReceiveMoveCompleted.AddDynamic(this, &UFWBTTask_MoveToTarget::OnTargetReached);
			return EBTNodeResult::Succeeded;
		}
	}
	else
	{
		FVector TargetLocation = OwnerComponent.GetBlackboardComponent()->GetValueAsVector(BlackboardKey.SelectedKeyName);
		Controller->MoveToLocation(TargetLocation, AcceptanceRadius, true, true, false, bCanStrafe);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}

void UFWBTTask_MoveToTarget::OnTargetReached(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	OwnerComp->GetBlackboardComponent()->ClearValue(BlackboardKey.SelectedKeyName);
}

