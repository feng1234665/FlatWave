// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/FWCharacter.h"
#include "FWWeaponData.h"
#include "FWDelegates.h"
#include "FWPlayerCharacterBase.generated.h"

UCLASS()
class FLATWAVE_API AFWPlayerCharacterBase : public AFWCharacter
{
	GENERATED_BODY()
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FirstPersonCameraComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USceneComponent* WeaponComponentParent;
public:
	AFWPlayerCharacterBase();

protected:
	virtual void BeginPlay();

	UPROPERTY(EditDefaultsOnly)
		float DodgeVerticalPart = 0.1f;
	UPROPERTY(EditDefaultsOnly)
		float DodgeVelocity = 1000.f;
	bool bCanDodge = true;
	void Landed(const FHitResult& Hit);

	UPROPERTY(EditDefaultsOnly)
		FVector WeaponOffset;
	class UFWPlayerWeaponBase* CurrentWeapon;
	UPROPERTY(EditDefaultsOnly)
		TArray<class UFWWeaponData*> Weapons;
	TMap<EWeaponType, class UFWPlayerWeaponBase*> WeaponComponents;
public:
	void EquipWeapon(int32 Index);
	TMap<EWeaponType, class UFWPlayerWeaponBase*> GetWeapons() const;
protected:
	float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	UPROPERTY(BlueprintAssignable)
		FOnAction OnDamageTaken;

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

	void MoveForward(float Value);
	void MoveRight(float Value);

	void JumpPressed();
	void JumpReleased();

	void DodgePressed();
public:
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const
	{
		return FirstPersonCameraComponent;
	}

	FORCEINLINE class USceneComponent* GetWeaponComponentParent() const
	{
		return WeaponComponentParent;
	}
};
