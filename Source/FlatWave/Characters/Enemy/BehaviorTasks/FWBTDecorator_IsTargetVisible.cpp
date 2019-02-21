// Fill out your copyright notice in the Description page of Project Settings.

#include "FWBTDecorator_IsTargetVisible.h"
#include "Tank/FWEnemyHoverTank.h"
#include "FWAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

bool UFWBTDecorator_IsTargetVisible::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
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
				FHitResult Hit;
				FVector Start = TargetActor->GetActorLocation() + CheckOffset;
				FVector End = Start + (TargetActor->GetActorForwardVector() * 5000.f) + CheckOffset;
				if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECollisionChannel::ECC_Visibility))
				{
					return Hit.Actor.Get() == TargetActor;
				}
				return true;
			}
		}
	}
	return false;
}
