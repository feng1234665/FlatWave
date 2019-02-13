// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/FWCharacter.h"
#include "FWPlayerCharacterBase.generated.h"

/**
 *
 */
UCLASS()
class FLATWAVE_API AFWPlayerCharacterBase : public AFWCharacter
{
	GENERATED_BODY()
protected:
	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USkeletalMeshComponent* Mesh1P;

	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USkeletalMeshComponent* FP_Gun;

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USceneComponent* FP_MuzzleLocation;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FirstPersonCameraComponent;

	// WEAPONS
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
		class UFWRocketLauncher* RocketLauncherComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
		class UFWMinigun* MinigunComponent;

	// STATUS

public:
	AFWPlayerCharacterBase();

protected:
	virtual void BeginPlay();

	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	class UFWWeaponBase* CurrentWeapon;
	float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
public:
	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector GunOffset;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class UAnimMontage* FireAnimation;

	FVector GetProjectileSpawnLocation();
	FRotator GetProjectileSpawnRotation();

	void OnTriggerPressed();
	void OnTriggerReleased();

	void OnAltTriggerPressed();
	void OnAltTriggerReleased();

	void SwitchToFirstWeapon();
	void SwitchToSecondWeapon();

	void MoveForward(float Value);
	void MoveRight(float Value);
public:
	/** Returns Mesh1P subobject **/
	FORCEINLINE class USkeletalMeshComponent* GetMesh1P() const
	{
		return Mesh1P;
	}
	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const
	{
		return FirstPersonCameraComponent;
	}
};
