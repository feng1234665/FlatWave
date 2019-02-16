// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "FWPlayerWeaponBase.generated.h"

/**
 *
 */
UCLASS()
class FLATWAVE_API UFWPlayerWeaponBase : public UStaticMeshComponent
{
	GENERATED_BODY()
public:
	UFWPlayerWeaponBase();

	virtual void BeginPlay() override;

	virtual void Init(class UFWWeaponData* WeaponData, FVector Offset);

	virtual void TriggerPressed();
	virtual void TriggerReleased();

	virtual class AFWProjectile* FireProjectile();

	void ChangeAmmo(int32 Amount);

	UFUNCTION(BlueprintCallable, BlueprintPure)
		int32 GetCurrentAmmo();
	UFUNCTION(BlueprintCallable, BlueprintPure)
		int32 GetMaxAmmo();
	void ConsumeAmmo();

	class UFWWeaponData* GetWeaponData() const;

protected:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction);

	float FireRateCounter = 0.f;
	float WarmupCounter = 0.f;
	bool bStartCounters = false;
	bool bTriggerPressed = false;

	UPROPERTY(EditDefaultsOnly)
		class UFWWeaponData* WeaponData;
	int32 CurrentAmmo = 0;

	FVector GetProjectileSpawnLocation();

	class AFWPlayerCharacterBase* GetOwnerCharacter() const;
	class AFWPlayerController* GetOwnerPlayerController() const;

	template<class T>
	T* GetWeaponDataAs() const;
};

template<class T>
T* UFWPlayerWeaponBase::GetWeaponDataAs() const
{
	return Cast<T>(WeaponData);
}
