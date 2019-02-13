// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FWHealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAction);

UCLASS(ClassGroup = (Custom))
class FLATWAVE_API UFWHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFWHealthComponent();

protected:
	virtual void BeginPlay() override;
	UPROPERTY(EditDefaultsOnly, Replicated, meta = (AllowPrivateAccess = "true"))
		float MaxHealth = 1000;
	UPROPERTY(Replicated)
		float CurrentHealth = 1000;
public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
		float GetHealth();
	UFUNCTION(BlueprintCallable, BlueprintPure)
		float GetHealthPercent();
	void ChangeHealth(float Amount);
	void Reset();

	UPROPERTY(BlueprintAssignable)
		FOnAction OnFullHealth;

	UPROPERTY(BlueprintAssignable)
		FOnAction OnDeath;
};