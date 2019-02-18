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
	if (GetPawn())
		GetPlayerPawn()->OnTriggerPressed();
}

void AFWPlayerController::OnTriggerReleased()
{
	if (GetPawn())
		GetPlayerPawn()->OnTriggerReleased();
}

void AFWPlayerController::OnAltTriggerPressed()
{
	if (GetPawn())
		GetPlayerPawn()->OnAltTriggerPressed();
}

void AFWPlayerController::OnAltTriggerReleased()
{
	if (GetPawn())
		GetPlayerPawn()->OnAltTriggerReleased();
}

void AFWPlayerController::SwitchToFirstWeapon()
{
	if (GetPawn())
		GetPlayerPawn()->SwitchToFirstWeapon();
}

void AFWPlayerController::SwitchToSecondWeapon()
{
	if (GetPawn())
		GetPlayerPawn()->SwitchToSecondWeapon();
}

void AFWPlayerController::MoveForward(float Value)
{
	if (GetPawn())
		GetPlayerPawn()->MoveForward(Value);
}

void AFWPlayerController::MoveRight(float Value)
{
	if (GetPawn())
		GetPlayerPawn()->MoveRight(Value);
}

void AFWPlayerController::Jump()
{
	if (GetPawn())
		GetPlayerPawn()->JumpPressed();
}

void AFWPlayerController::StopJump()
{
	if (GetPawn())
		GetPlayerPawn()->JumpReleased();
}

class AFWPlayerCharacterBase* AFWPlayerController::GetPlayerPawn() const
{
	return Cast<AFWPlayerCharacterBase>(GetPawn());
}
