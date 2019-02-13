// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FWSpawner.generated.h"

UCLASS()
class FLATWAVE_API AFWSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFWSpawner();

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Debug, meta = (AllowPrivateAccess = "true"))
		class USceneComponent* Root;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Debug, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* VisualDebugMeshComponent;

	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
