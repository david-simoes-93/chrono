// Copyright Epic Games, Inc. All Rights Reserved.

#include "TP_WeaponComponent.h"
#include "ChronoCharacter.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Sets default values for this component's properties
UTP_WeaponComponent::UTP_WeaponComponent()
{
	// Default offset from the character location for projectiles to spawn
	MuzzleOffset = FVector(70.0f, 18.0f, 8.0f);
	_laser_type = WeaponLaserType::PAUSE;
}

void UTP_WeaponComponent::Fire()
{
	UE_LOG(LogTemp, Warning, TEXT("fire"));
	if (Character == nullptr || Character->GetController() == nullptr)
	{
		return;
	}

	TSubclassOf<class ALaserPause> laser_bp;
	switch (_laser_type)
	{
	case WeaponLaserType::RESET:
		laser_bp = nullptr;
		break;
	case WeaponLaserType::PAUSE:
		laser_bp = pause_laser_bp;
		break;
	case WeaponLaserType::REVERT:
		laser_bp = nullptr;
		break;
	case WeaponLaserType::SPEED:
		laser_bp = nullptr;
		break;
	}

	// Try and fire a laser
	if (laser_bp != nullptr)
	{
		UWorld *const World = GetWorld();
		if (World != nullptr)
		{
			APlayerController *PlayerController = Cast<APlayerController>(Character->GetController());
			const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);

			// Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			// Spawn the projectile at the muzzle

			World->SpawnActor<ALaserPause>(laser_bp, SpawnLocation, SpawnRotation, ActorSpawnParams);
		}
	}

	// Try and play the sound if specified
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
	}

	// Try and play a firing animation if specified
	if (FireAnimation != nullptr)
	{
		// Get the animation object for the arms mesh
		UAnimInstance *AnimInstance = Character->GetMesh1P()->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}

void UTP_WeaponComponent::AttachWeapon(AChronoCharacter *TargetCharacter)
{
	Character = TargetCharacter;
	if (Character == nullptr)
	{
		return;
	}

	// Attach the weapon to the First Person Character
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(Character->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));

	// switch bHasRifle so the animation blueprint can switch to another animation set
	Character->SetHasRifle(true);

	// Set up action bindings
	if (APlayerController *PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem *Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when using touch input
			Subsystem->AddMappingContext(FireMappingContext, 1);
		}

		if (UEnhancedInputComponent *EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			// Fire
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &UTP_WeaponComponent::Fire);

			// Cycle laser type
			EnhancedInputComponent->BindAction(CycleActionNext, ETriggerEvent::Triggered, this, &UTP_WeaponComponent::CycleNext);
			EnhancedInputComponent->BindAction(CycleActionPrevious, ETriggerEvent::Triggered, this, &UTP_WeaponComponent::CyclePrevious);
		}
	}
}

void UTP_WeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (Character == nullptr)
	{
		return;
	}

	if (APlayerController *PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem *Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(FireMappingContext);
		}
	}
}

void UTP_WeaponComponent::CycleNext()
{
	switch (_laser_type)
	{
	case WeaponLaserType::RESET:
		_laser_type = WeaponLaserType::PAUSE;
		laser_color = FColor::Blue;
		UE_LOG(LogTemp, Warning, TEXT("Laser type: PAUSE"));
		break;
	case WeaponLaserType::PAUSE:
		_laser_type = WeaponLaserType::REVERT;
		laser_color = FColor::Red;
		UE_LOG(LogTemp, Warning, TEXT("Laser type: REVERT"));
		break;
	case WeaponLaserType::REVERT:
		_laser_type = WeaponLaserType::SPEED;
		laser_color = FColor::Green;
		UE_LOG(LogTemp, Warning, TEXT("Laser type: SPEED"));
		break;
	case WeaponLaserType::SPEED:
		_laser_type = WeaponLaserType::RESET;
		laser_color = FColor::White;
		UE_LOG(LogTemp, Warning, TEXT("Laser type: RESET"));
		break;
	}
}

void UTP_WeaponComponent::CyclePrevious()
{
	switch (_laser_type)
	{
	case WeaponLaserType::RESET:
		_laser_type = WeaponLaserType::SPEED;
		laser_color = FColor::Green;
	case WeaponLaserType::SPEED:
		_laser_type = WeaponLaserType::REVERT;
		laser_color = FColor::Red;
	case WeaponLaserType::REVERT:
		_laser_type = WeaponLaserType::PAUSE;
		laser_color = FColor::Blue;
	case WeaponLaserType::PAUSE:
		_laser_type = WeaponLaserType::RESET;
		laser_color = FColor::White;
	}
}