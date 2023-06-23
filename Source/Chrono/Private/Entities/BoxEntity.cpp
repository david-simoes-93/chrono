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
	last_move = delta_move;

	// if we hit Character, move it along
	if (sweep_hit_result.bBlockingHit)
	{
		// move actor out of the way
		sweep_hit_result.GetActor()->SetActorLocation(sweep_hit_result.GetActor()->GetActorLocation() + delta_move);

		// TP to intended location
		SetActorLocation(final_location, true, &sweep_hit_result, ETeleportType::None);
		if (sweep_hit_result.bBlockingHit)
		{
			UE_LOG(LogTemp, Warning, TEXT("2nd collision detected!"));
		}

		/*
		TODO: find a way to only move Character actor
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
	// store actor, and OnDestroy, do stuff to actor? or just ignore all this non-sense and have actor jump lol
	// also, what happens when platform moves with actor on top?
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
