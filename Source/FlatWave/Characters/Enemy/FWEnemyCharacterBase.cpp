// Fill out your copyright notice in the Description page of Project Settings.

#include "FWEnemyCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "FWAIController.h"
#include "TimerManager.h"

void AFWEnemyCharacterBase::OnDeath()
{
	if (GetController())
		GetController()->UnPossess();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, this, &AFWEnemyCharacterBase::SimpleDestroy, 1.f, false, TimeToDestroy);
}

void AFWEnemyCharacterBase::SimpleDestroy()
{
	Destroy();
}

float AFWEnemyCharacterBase::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	if (DamageCauser->Instigator != this && !Cast<AFWAIController>(EventInstigator))
	{
		return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	}
	return 0.f;
}
