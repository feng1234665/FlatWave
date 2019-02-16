// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "FWAIController.generated.h"

/**
 * 
 */
UCLASS()
class FLATWAVE_API AFWAIController : public AAIController
{
	GENERATED_BODY()
public:
	AFWAIController();
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UBehaviorTreeComponent* BehaviorComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UBlackboardComponent* BlackboardComponent;

	virtual void BeginPlay() override;
	void Possess(class APawn* InPawn) override;
	void UnPossess() override;

	FName TargetPlayerKeyName;

public:

	void SetTargetPlayer(class AFWPlayerCharacterBase* Player);
};
