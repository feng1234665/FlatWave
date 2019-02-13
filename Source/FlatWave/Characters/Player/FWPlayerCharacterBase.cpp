// Fill out your copyright notice in the Description page of Project Settings.

#include "FWPlayerCharacterBase.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "FWRocketLauncher.h"
#include "FWMinigun.h"
#include "FWHealthComponent.h"
#include "FWPlayerController.h"

DEFINE_LOG_CATEGORY_STATIC(LogFWPlayerCharacter, Warning, All);

AFWPlayerCharacterBase::AFWPlayerCharacterBase()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	Mesh1P->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);

	// Create a gun mesh component
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	// FP_Gun->SetupAttachment(Mesh1P, TEXT("GripPoint"));
	FP_Gun->SetupAttachment(RootComponent);

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FP_Gun);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 0.0f, 10.0f);

	RocketLauncherComponent = CreateDefaultSubobject<UFWRocketLauncher>("RocketLauncher");
	MinigunComponent = CreateDefaultSubobject<UFWMinigun>("Minigun");


}

void AFWPlayerCharacterBase::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));

	Mesh1P->SetHiddenInGame(false, true);

	CurrentWeapon = MinigunComponent;
}

void AFWPlayerCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AFWPlayerCharacterBase::OnTriggerPressed);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AFWPlayerCharacterBase::OnTriggerReleased);

	PlayerInputComponent->BindAxis("MoveForward", this, &AFWPlayerCharacterBase::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFWPlayerCharacterBase::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
}

float AFWPlayerCharacterBase::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	if (!Cast<AFWPlayerController>(EventInstigator))
	{
		return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	}
	return 0.f;
}

FVector AFWPlayerCharacterBase::GetProjectileSpawnLocation()
{
	return FP_MuzzleLocation->GetComponentLocation();
}

FRotator AFWPlayerCharacterBase::GetProjectileSpawnRotation()
{
	return GetControlRotation();
}

void AFWPlayerCharacterBase::OnTriggerPressed()
{
	if (CurrentWeapon)
		CurrentWeapon->TriggerPressed();

	// try and play a firing animation if specified
	if (FireAnimation != NULL)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
		if (AnimInstance != NULL)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}

void AFWPlayerCharacterBase::OnTriggerReleased()
{
	if (CurrentWeapon)
	CurrentWeapon->TriggerReleased();
}

void AFWPlayerCharacterBase::OnAltTriggerPressed()
{

}

void AFWPlayerCharacterBase::OnAltTriggerReleased()
{

}

void AFWPlayerCharacterBase::SwitchToFirstWeapon()
{
	if (CurrentWeapon && CurrentWeapon != MinigunComponent)
	{
		CurrentWeapon->TriggerReleased();
	}
	CurrentWeapon = MinigunComponent;
}

void AFWPlayerCharacterBase::SwitchToSecondWeapon()
{
	if (CurrentWeapon && CurrentWeapon != RocketLauncherComponent)
	{
		CurrentWeapon->TriggerReleased();
	}
	CurrentWeapon = RocketLauncherComponent;
}

void AFWPlayerCharacterBase::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AFWPlayerCharacterBase::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}
