// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FWProjectile.generated.h"

UCLASS()
class FLATWAVE_API AFWProjectile : public AActor
{
	GENERATED_BODY()
public:
	AFWProjectile();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* ProjectileMesh;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
public:
	void Init(class UFWProjectileData* NewProctileData);

	UPROPERTY(EditDefaultsOnly)
		class UFWProjectileData* ProjectileData;

	void NotifyActorBeginOverlap(AActor* OtherActor);
};
