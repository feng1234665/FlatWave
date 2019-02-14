// Fill out your copyright notice in the Description page of Project Settings.

#include "FWPlayerController.h"
#include "FWPlayerCharacterBase.h"
#include "FWUtilities.h"

void AFWPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction("Jump", IE_Pressed, this, &AFWPlayerController::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &AFWPlayerController::StopJump);

	InputComponent->BindAction("Fire", IE_Pressed, this, &AFWPlayerController::OnTriggerPressed);
	InputComponent->BindAction("Fire", IE_Released, this, &AFWPlayerController::OnTriggerReleased);

	InputComponent->BindAction("AltFire", IE_Pressed, this, &AFWPlayerController::OnAltTriggerPressed);
	InputComponent->BindAction("AltFire", IE_Released, this, &AFWPlayerController::OnAltTriggerReleased);

	InputComponent->BindAction("SwitchToFirstWeapon", IE_Pressed, this, &AFWPlayerController::SwitchToFirstWeapon);
	InputComponent->BindAction("SwitchToSecondWeapon", IE_Pressed, this, &AFWPlayerController::SwitchToSecondWeapon);

	InputComponent->BindAxis("MoveForward", this, &AFWPlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AFWPlayerController::MoveRight);

	InputComponent->BindAxis("Turn", this, &APlayerController::AddYawInput);
	InputComponent->BindAxis("LookUp", this, &APlayerController::AddPitchInput);
}

void AFWPlayerController::OnTriggerPressed()
{
	GetPlayerPawn()->OnTriggerPressed();
}

void AFWPlayerController::OnTriggerReleased()
{
	GetPlayerPawn()->OnTriggerReleased();
}

void AFWPlayerController::OnAltTriggerPressed()
{
	GetPlayerPawn()->OnAltTriggerPressed();
}

void AFWPlayerController::OnAltTriggerReleased()
{
	GetPlayerPawn()->OnAltTriggerReleased();
}

void AFWPlayerController::SwitchToFirstWeapon()
{
	GetPlayerPawn()->SwitchToFirstWeapon();
}

void AFWPlayerController::SwitchToSecondWeapon()
{
	GetPlayerPawn()->SwitchToSecondWeapon();
}

void AFWPlayerController::MoveForward(float Value)
{
	GetPlayerPawn()->MoveForward(Value);
}

void AFWPlayerController::MoveRight(float Value)
{
	GetPlayerPawn()->MoveRight(Value);
}

void AFWPlayerController::Jump()
{
	GetPlayerPawn()->Jump();
}

void AFWPlayerController::StopJump()
{
	GetPlayerPawn()->StopJumping();
}

class AFWPlayerCharacterBase* AFWPlayerController::GetPlayerPawn() const
{
	return Cast<AFWPlayerCharacterBase>(GetPawn());
}
