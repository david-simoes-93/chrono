// Fill out your copyright notice in the Description page of Project Settings.

#include "Entities/VerticalBoxes.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
AVerticalBoxes::AVerticalBoxes() : _elapsed_spawn_time{0}, _elapsed_despawn_time{0}
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AVerticalBoxes::BeginPlay()
{
	Super::BeginPlay();
	_elapsed_despawn_time = _distance / _box_speed;
}

// Called every frame
void AVerticalBoxes::Tick(float delta_time)
{
	UWorld *const world = GetWorld();
	if (world == nullptr)
	{
		return;
	}
	if (_box_entity == nullptr)
	{
		return;
	}

	Super::Tick(delta_time);

	if (_current_state == LaserType::PAUSE)
	{
		return;
	}
	else if (_current_state == LaserType::SPEED)
	{
		delta_time *= 2;
	}
	else if (_current_state == LaserType::REVERT)
	{
		delta_time *= -1;
	}

	_elapsed_spawn_time += delta_time;
	_elapsed_despawn_time += delta_time;
	spawnBox(world);

	moveBoxes(delta_time);
}

void AVerticalBoxes::spawnBox(UWorld *const world)
{
	const FRotator SpawnRotation = GetActorRotation();
	FVector SpawnLocation;
	if (_current_state != LaserType::REVERT)
	{
		if (_elapsed_spawn_time < _spawn_period)
		{
			return;
		}
		SpawnLocation = GetActorLocation() - SpawnRotation.RotateVector(FVector{0, 0, _distance_boxes_from_spawn}); // Spawn the box just below the spawner
	}
	else
	{
		if (_elapsed_despawn_time > 0)
		{
			return;
		}
		SpawnLocation = GetActorLocation() + SpawnRotation.RotateVector(FVector{0, 0, _distance + _distance_boxes_from_spawn}); // Spawn the box just above the despawner
	}

	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding;

	ABoxEntity *new_box = world->SpawnActor<ABoxEntity>(_box_entity, SpawnLocation, SpawnRotation, ActorSpawnParams);

	if (new_box != nullptr)
	{
		// UE_LOG(LogTemp, Warning, TEXT("spawned at %s"), *new_box->GetActorLocation().ToString());
		new_box->setParent(this);
		if (_current_state != LaserType::REVERT)
		{
			_boxes.push_back(new_box);
			_elapsed_spawn_time = 0;
		}
		else
		{
			_boxes.push_front(new_box);
			_elapsed_despawn_time = _spawn_period;
		}
	}
}

void AVerticalBoxes::moveBoxes(float delta_time)
{
	if (_boxes.empty())
	{
		return;
	}

	const auto delta_movement = GetActorRotation().RotateVector({0, 0, _box_speed * delta_time});

	for (const auto &box_ptr : _boxes)
	{
		box_ptr->move(delta_movement);
	}

	if (_current_state != LaserType::REVERT)
	{
		if (FVector::Distance(_boxes.front()->GetActorLocation(), GetActorLocation()) > _distance + _distance_boxes_from_spawn * 2)
		{
			_boxes.front()->Destroy();
			_boxes.pop_front();
			_elapsed_despawn_time = 0;
		}
	}
	else
	{
		if (FVector::Distance(_boxes.back()->GetActorLocation(), GetActorLocation() + FVector{0, 0, _distance + _distance_boxes_from_spawn}) > _distance + _distance_boxes_from_spawn * 2)
		{
			_boxes.back()->Destroy();
			_boxes.pop_back();
			_elapsed_spawn_time = _spawn_period;
		}
	}
}

void AVerticalBoxes::setPause()
{
	_current_state = LaserType::PAUSE;
}

void AVerticalBoxes::setReset()
{
	_current_state = LaserType::RESET;
}

void AVerticalBoxes::setSpeed()
{
	_current_state = LaserType::SPEED;
}

void AVerticalBoxes::setReverse()
{
	_current_state = LaserType::REVERT;
}
