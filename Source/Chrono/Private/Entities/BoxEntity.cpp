// Fill out your copyright notice in the Description page of Project Settings.

#include "Entities/BoxEntity.h"

#include "FPS/ChronoCharacter.h"

// Sets default values
ABoxEntity::ABoxEntity()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ABoxEntity::BeginPlay()
{
	Super::BeginPlay();
}

void ABoxEntity::setParent(AActor *parent)
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

void ABoxEntity::move(const FVector &delta_move)
{
	FHitResult sweep_hit_result;
	const FVector final_location = GetActorLocation() + delta_move;
	SetActorLocation(final_location, true, &sweep_hit_result, ETeleportType::None);

	// if we hit Character, move it along
	if (sweep_hit_result.bBlockingHit || sweep_hit_result.bStartPenetrating || sweep_hit_result.GetActor())
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

		/*
		TODO: find a way to move only AChronoCharacter actor
		if (sweep_hit_result.GetActor()->GetClass()->ImplementsInterface(AChronoCharacter::StaticClass()))
		{
			if (AChronoCharacter *character_actor = Cast<AChronoCharacter>(sweep_hit_result.GetActor()))
			{
				character_actor->SetActorLocation(character_actor->GetActorLocation() + delta_move);
				SetActorLocation(final_location, true, &sweep_hit_result, ETeleportType::None);
			}
		}
		*/
	}
}

void ABoxEntity::setPause()
{
	if (!_pause_parent)
	{
		return;
	}
	_pause_parent->setPause();
}

void ABoxEntity::setReset()
{
	if (!_reset_parent)
	{
		return;
	}
	_reset_parent->setReset();
}

void ABoxEntity::setSpeed()
{
	if (!_speed_parent)
	{
		return;
	}
	_speed_parent->setSpeed();
}

void ABoxEntity::setReverse()
{
	if (!_reverse_parent)
	{
		return;
	}
	_reverse_parent->setReverse();
}
