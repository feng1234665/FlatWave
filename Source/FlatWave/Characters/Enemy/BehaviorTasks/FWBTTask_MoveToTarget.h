// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Navigation/PathFollowingComponent.h"
#include "FWBTTask_MoveToTarget.generated.h"

/**
 *
 */
UCLASS()
class FLATWAVE_API UFWBTTask_MoveToTarget : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

private:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComponent, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere)
		float AcceptanceRadius = 100.f;
	UPROPERTY(EditAnywhere)
		bool bCanStrafe = false;
	UPROPERTY(EditAnywhere)
		bool bResetBlackboardKeyOnReached = true;

	UBehaviorTreeComponent* OwnerComp;
	UFUNCTION()
		void OnTargetReached(FAIRequestID RequestID, EPathFollowingResult::Type Result);
};
