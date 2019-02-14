// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "FWPlayerController.generated.h"

UCLASS()
class FLATWAVE_API AFWPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	void SetupInputComponent() override;

private:
	void OnTriggerPressed();
	void OnTriggerReleased();

	void OnAltTriggerPressed();
	void OnAltTriggerReleased();

	void SwitchToFirstWeapon();
	void SwitchToSecondWeapon();

	void MoveForward(float Value);
	void MoveRight(float Value);

	void Jump();
	void StopJump();

	class AFWPlayerCharacterBase* GetPlayerPawn() const;
};
