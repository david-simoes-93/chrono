// Fill out your copyright notice in the Description page of Project Settings.

#include "Entities/PistonEntity.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "FPS/ChronoCharacter.h"

// Sets default values
APistonEntity::APistonEntity()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void APistonEntity::BeginPlay()
{
	Super::BeginPlay();
}

void APistonEntity::setParent(AActor *parent)
{
	if (_pause_parent || _reset_parent || _speed_parent)
	{
		return;
	}
	_pause_parent = Cast<IPausable>(parent);
	_reset_parent = Cast<IResettable>(parent);
	_speed_parent = Cast<ISpeedable>(parent);
}

void APistonEntity::move(const FVector &speed, const FVector &delta_move)
{
	FHitResult sweep_hit_result;
	const FVector final_location = GetActorLocation() + delta_move;
	SetActorLocation(final_location, true, &sweep_hit_result, ETeleportType::None);

	// if we hit Character, move it along (collisions set up such that we only collide with player)
	if (sweep_hit_result.bBlockingHit)
	{
		// move actor out of the way
		AActor *player_ptr = sweep_hit_result.GetActor();
		FHitResult pawn_sweep_hit_result;
		player_ptr->SetActorLocation(player_ptr->GetActorLocation() + delta_move, true, &pawn_sweep_hit_result, ETeleportType::ResetPhysics);

		// Launch actor with same speed as piston
		AChronoCharacter *Character = Cast<AChronoCharacter>(player_ptr);
		Character->LaunchCharacter(speed, true, true);

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

void APistonEntity::setPause()
{
	if (!_pause_parent)
	{
		return;
	}
	_pause_parent->setPause();
}

void APistonEntity::setReset()
{
	if (!_reset_parent)
	{
		return;
	}
	_reset_parent->setReset();
}

void APistonEntity::setSpeed()
{
	if (!_speed_parent)
	{
		return;
	}
	_speed_parent->setSpeed();
}
