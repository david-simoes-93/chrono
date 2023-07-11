// Fill out your copyright notice in the Description page of Project Settings.

#include "Entities/PistonController.h"

// Sets default values
APistonController::APistonController() : _elapsed_thrust_time{0}, _piston_thrusting_forward{false}
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APistonController::BeginPlay()
{
	Super::BeginPlay();

	UWorld *const world = GetWorld();
	if (world == nullptr)
	{
		return;
	}
	spawnPiston(world);
}

// Called every frame
void APistonController::Tick(float delta_time)
{
	if (_piston == nullptr)
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

	_elapsed_thrust_time += delta_time;

	thrustPiston(delta_time);
}

void APistonController::spawnPiston(UWorld *const world)
{
	FVector SpawnLocation = getPistonSpawnLocation(); // Spawn the piston just below the spawner
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding;
	auto new_piston = world->SpawnActor<APistonEntity>(_entity, SpawnLocation, GetActorRotation(), ActorSpawnParams);

	if (new_piston != nullptr)
	{
		// UE_LOG(LogTemp, Warning, TEXT("spawned at %s"), *new_piston->GetActorLocation().ToString());
		new_piston->setParent(this);
		_piston = new_piston;
	}
}

void APistonController::thrustPiston(float delta_time)
{
	if (_piston == nullptr)
	{
		return;
	}

	// move piston
	FVector delta_movement;
	if (_piston_thrusting_forward)
	{
		delta_movement = GetActorRotation().RotateVector({0, 0, _entity_forward_speed * delta_time});
	}
	else if (!_piston_thrusting_forward && !isPistonBehindSpawn())
	{
		delta_movement = GetActorRotation().RotateVector({0, 0, -_entity_back_speed * delta_time});
	}

	_piston->move(delta_movement);

	// reverse thrust direction
	if (_piston_thrusting_forward && FVector::Distance(_piston->GetActorLocation(), getPistonSpawnLocation()) > getPistonTravelDistance())
	{
		_piston_thrusting_forward = false;
	}
	else if (!_piston_thrusting_forward && _elapsed_thrust_time > _piston_period && isPistonBehindSpawn())
	{
		_piston_thrusting_forward = true;
		_elapsed_thrust_time = true;
	}
}

void APistonController::setPause()
{
	_current_state = LaserType::PAUSE;
}

void APistonController::setReset()
{
	_current_state = LaserType::RESET;
}

void APistonController::setSpeed()
{
	_current_state = LaserType::SPEED;
}
