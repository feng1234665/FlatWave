// Fill out your copyright notice in the Description page of Project Settings.

#include "FWPlayerController.h"
#include "FWPlayerCharacterBase.h"
#include "FWUtilities.h"
#include "FWProjectile.h"
#include "FWProjectileData.h"
#include "FWWeaponData.h"
#include "FWMainGameMode.h"

AFWPlayerController::AFWPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AFWPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction("Jump", IE_Pressed, this, &AFWPlayerController::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &AFWPlayerController::StopJump);

	InputComponent->BindAction("Dodge", IE_Pressed, this, &AFWPlayerController::DodgePressed);

	InputComponent->BindAction("Fire", IE_Pressed, this, &AFWPlayerController::OnTriggerPressed);
	InputComponent->BindAction("Fire", IE_Released, this, &AFWPlayerController::OnTriggerReleased);

	InputComponent->BindAction("AltFire", IE_Pressed, this, &AFWPlayerController::OnAltTriggerPressed);
	InputComponent->BindAction("AltFire", IE_Released, this, &AFWPlayerController::OnAltTriggerReleased);

	InputComponent->BindAction("SwitchToFirstWeapon", IE_Pressed, this, &AFWPlayerController::SwitchToFirstWeapon);
	InputComponent->BindAction("SwitchToSecondWeapon", IE_Pressed, this, &AFWPlayerController::SwitchToSecondWeapon);
	InputComponent->BindAction("SwitchToThirdWeapon", IE_Pressed, this, &AFWPlayerController::SwitchToThirdWeapon);

	InputComponent->BindAction("PauseGame", IE_Pressed, this, &AFWPlayerController::PausePressed).bExecuteWhenPaused = true;

	InputComponent->BindAxis("MoveForward", this, &AFWPlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AFWPlayerController::MoveRight);

	InputComponent->BindAxis("Turn", this, &AFWPlayerController::TurnYaw);
	InputComponent->BindAxis("LookUp", this, &AFWPlayerController::TurnPitch);
}

void AFWPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AFWPlayerController::Enable()
{
	EnableInput(this);
}

void AFWPlayerController::Disable()
{
	DisableInput(this);
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
		GetPlayerPawn()->EquipWeapon(0);
}

void AFWPlayerController::SwitchToSecondWeapon()
{
	if (GetPawn())
		GetPlayerPawn()->EquipWeapon(1);
}

void AFWPlayerController::SwitchToThirdWeapon()
{
	if (GetPawn())
		GetPlayerPawn()->EquipWeapon(2);
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

void AFWPlayerController::TurnYaw(float Value)
{
	AddYawInput(Value);
}

void AFWPlayerController::TurnPitch(float Value)
{
	AddPitchInput(Value);
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

void AFWPlayerController::DodgePressed()
{
	if (GetPawn())
		GetPlayerPawn()->DodgePressed();
}

void AFWPlayerController::PausePressed()
{
	UFWUtilities::GetFWMainGameMode(this)->TogglePause();
}

class AFWPlayerCharacterBase* AFWPlayerController::GetPlayerPawn() const
{
	return Cast<AFWPlayerCharacterBase>(GetPawn());
}
