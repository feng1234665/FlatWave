// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/FWCharacter.h"
#include "FWWeaponData.h"
#include "FWPlayerCharacterBase.generated.h"

UCLASS()
class FLATWAVE_API AFWPlayerCharacterBase : public AFWCharacter
{
	GENERATED_BODY()
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FirstPersonCameraComponent;
public:
	AFWPlayerCharacterBase();

protected:
	virtual void BeginPlay();

	UPROPERTY(EditDefaultsOnly)
		FVector WeaponOffset;
	class UFWPlayerWeaponBase* CurrentWeapon;
	UPROPERTY(EditDefaultsOnly)
		TArray<class UFWWeaponData*> Weapons;
	TMap<EWeaponType, class UFWPlayerWeaponBase*> WeaponComponents;
	float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(EditDefaultsOnly, Category = Audio)
		class USoundBase* JumpSound;

	void OnDeath() override;
public:
	UFUNCTION(BlueprintCallable)
		class UFWPlayerWeaponBase* GetCurrentWeapon() const;

	FRotator GetProjectileSpawnRotation();

	void OnTriggerPressed();
	void OnTriggerReleased();

	void OnAltTriggerPressed();
	void OnAltTriggerReleased();

	void SwitchToFirstWeapon();
	void SwitchToSecondWeapon();

	void MoveForward(float Value);
	void MoveRight(float Value);

	void JumpPressed();
	void JumpReleased();
public:
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const
	{
		return FirstPersonCameraComponent;
	}
};
