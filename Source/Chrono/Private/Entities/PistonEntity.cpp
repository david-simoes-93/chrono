// Fill out your copyright notice in the Description page of Project Settings.

#include "Entities/PistonEntity.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

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
		AActor *actor_ptr = sweep_hit_result.GetActor();
		launchCharacter(Cast<AChronoCharacter>(actor_ptr), speed, delta_move);
		moveFragileBox(Cast<AFragileBox>(actor_ptr), delta_move);

		// TP to intended location, should be clear now
		SetActorLocation(final_location, true, &sweep_hit_result, ETeleportType::None);
		if (sweep_hit_result.bBlockingHit)
		{
			UE_LOG(LogTemp, Warning, TEXT("2nd collision detected! probably blocked by a FragileBox that can't be destroyed"));
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

void APistonEntity::launchCharacter(AChronoCharacter *character, const FVector &speed, const FVector &delta_move)
{
	if (!character)
	{
		return;
	}

	FHitResult pawn_sweep_hit_result;
	character->SetActorLocation(character->GetActorLocation() + delta_move, true, &pawn_sweep_hit_result, ETeleportType::ResetPhysics);
	if (pawn_sweep_hit_result.bBlockingHit)
	{
		// actor has no place to be moved to
		// only if sped up as well? nah
		character->Destroy();
	}

	// Launch actor with same speed as piston
	character->LaunchCharacter(speed, true, true);
}
void APistonEntity::moveFragileBox(AFragileBox *box, const FVector &delta_move)
{
	if (!box)
	{
		return;
	}
	if (box->isPaused())
	{
		return;
	}

	FHitResult pawn_sweep_hit_result;
	box->SetActorLocation(box->GetActorLocation() + delta_move, true, &pawn_sweep_hit_result, ETeleportType::ResetPhysics);
	if (pawn_sweep_hit_result.bBlockingHit)
	{
		// actor has no place to be moved to
		// fragment it only if SPED UP
		if (isSpeeded())
		{
			box->OnFragmentation();
		}
	}
}
