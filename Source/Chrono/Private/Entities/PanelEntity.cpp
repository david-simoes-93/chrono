// Fill out your copyright notice in the Description page of Project Settings.

#include "Entities/PanelEntity.h"

#include "FPS/ChronoCharacter.h"

// Sets default values
APanelEntity::APanelEntity()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void APanelEntity::BeginPlay()
{
	Super::BeginPlay();
}

void APanelEntity::setParent(AActor *parent)
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

void APanelEntity::move(const FVector &delta_move, const FVector &world_scale)
{
	FHitResult sweep_hit_result;
	const FVector final_location = GetActorLocation() + delta_move;
	SetActorLocation(final_location, true, &sweep_hit_result, ETeleportType::None);
	SetActorScale3D(world_scale);

	// if we hit Character, move it along (collisions set up such that we only collide with player)
	if (sweep_hit_result.bBlockingHit)
	{
		// move actor out of the way
		auto player_ptr = sweep_hit_result.GetActor();
		FHitResult pawn_sweep_hit_result;
		player_ptr->SetActorLocation(player_ptr->GetActorLocation() + delta_move, true, &pawn_sweep_hit_result, ETeleportType::None);

		if (pawn_sweep_hit_result.bBlockingHit)
		{
			// get the hit direction, from the center of the pawn to the point where the capsule collider hit something
			FVector hit_direction_from_pawn = (pawn_sweep_hit_result.ImpactPoint - pawn_sweep_hit_result.Location);
			// calculate the angle between the movement done to Pawn, and the hit direction
			float collision_angle = FMath::Acos(hit_direction_from_pawn.GetSafeNormal().Dot(delta_move.GetSafeNormal()));
			// if the box is squishing a pawn against something, the angle should be close to 0. if it's not, then likely the pawn is being pushed
			//  against a ramp or something (lateral movement, but downwards collision), and so we don't kill him, we let physics put him in proper place
			if (collision_angle < 0.785) // 45º or PI/4 radians
			{
				// actor has no place to be moved to
				player_ptr->Destroy();
			}
		}

		// TP to intended location
		SetActorLocation(final_location, true, &sweep_hit_result, ETeleportType::None);
		if (sweep_hit_result.bBlockingHit)
		{
			UE_LOG(LogTemp, Warning, TEXT("2nd collision detected!"));
		}
	}
}

void APanelEntity::setPause()
{
	if (!_pause_parent)
	{
		return;
	}
	_pause_parent->setPause();
}

void APanelEntity::setReset()
{
	if (!_reset_parent)
	{
		return;
	}
	_reset_parent->setReset();
}

void APanelEntity::setSpeed()
{
	if (!_speed_parent)
	{
		return;
	}
	_speed_parent->setSpeed();
}

void APanelEntity::setReverse()
{
	if (!_reverse_parent)
	{
		return;
	}
	_reverse_parent->setReverse();
}
