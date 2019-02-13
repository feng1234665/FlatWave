// Fill out your copyright notice in the Description page of Project Settings.

#include "FWHealthComponent.h"
#include "Net/UnrealNetwork.h"

UFWHealthComponent::UFWHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bReplicates = true;
}

void UFWHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
}

float UFWHealthComponent::GetHealth()
{
	return CurrentHealth;
}

float UFWHealthComponent::GetHealthPercent()
{
	return (float)CurrentHealth / MaxHealth;
}

void UFWHealthComponent::ChangeHealth(float Amount)
{
	CurrentHealth = FMath::Clamp(CurrentHealth + Amount, 0.f, MaxHealth);
	if (CurrentHealth >= MaxHealth)
	{
		OnFullHealth.Broadcast();
	}
	else if (CurrentHealth <= 0.f)
	{
		OnDeath.Broadcast();
	}
}

void UFWHealthComponent::Reset()
{
	ChangeHealth(MaxHealth);
}

void UFWHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UFWHealthComponent, CurrentHealth);
	DOREPLIFETIME(UFWHealthComponent, MaxHealth);
}
