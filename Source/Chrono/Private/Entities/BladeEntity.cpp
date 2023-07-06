// Fill out your copyright notice in the Description page of Project Settings.

#include "Entities/BladeEntity.h"

// Sets default values
ABladeEntity::ABladeEntity()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ABladeEntity::BeginPlay()
{
	Super::BeginPlay();
}

void ABladeEntity::setParent(AActor *parent)
{
	if (_pause_parent || _reset_parent || _reverse_parent || _speed_parent)
	{
		return;
	}
	_pause_parent = Cast<IPausable>(parent);
	_reset_parent = Cast<IResettable>(parent);
	_reverse_parent = Cast<IReversible>(parent);
	_speed_parent = Cast<ISpeedable>(parent);
}

void ABladeEntity::move(const FVector &delta_move, const FQuat &delta_rotation)
{
	FHitResult sweep_hit_result;
	AddActorLocalRotation(delta_rotation, false, &sweep_hit_result, ETeleportType::None);
	const FVector final_location = GetActorLocation() + delta_move;
	SetActorLocation(final_location, true, &sweep_hit_result, ETeleportType::None);

	// if we hit Character, move it along (collisions set up such that we only collide with player)
	if (sweep_hit_result.bBlockingHit)
	{
		// move actor out of the way
		auto player_ptr = sweep_hit_result.GetActor();
		FHitResult pawn_sweep_hit_result;
		player_ptr->SetActorLocation(player_ptr->GetActorLocation() + delta_move, true, &pawn_sweep_hit_result, ETeleportType::None);

		if (pawn_sweep_hit_result.bBlockingHit)
		{
			// actor has no place to be moved to
			player_ptr->Destroy();
		}

		// TP to intended location
		SetActorLocation(final_location, true, &sweep_hit_result, ETeleportType::None);
		if (sweep_hit_result.bBlockingHit)
		{
			UE_LOG(LogTemp, Warning, TEXT("2nd collision detected!"));
		}
	}
}

void ABladeEntity::setPause()
{
	if (!_pause_parent)
	{
		return;
	}
	_pause_parent->setPause();
}

void ABladeEntity::setReset()
{
	if (!_reset_parent)
	{
		return;
	}
	_reset_parent->setReset();
}

void ABladeEntity::setSpeed()
{
	if (!_speed_parent)
	{
		return;
	}
	_speed_parent->setSpeed();
}

void ABladeEntity::setReverse()
{
	if (!_reverse_parent)
	{
		return;
	}
	_reverse_parent->setReverse();
}
