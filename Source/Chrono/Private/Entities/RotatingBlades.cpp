// Fill out your copyright notice in the Description page of Project Settings.

#include "Entities/RotatingBlades.h"

// Sets default values
ARotatingBlades::ARotatingBlades()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ARotatingBlades::BeginPlay()
{
	Super::BeginPlay();
	_elapsed_despawn_time = _distance / _entity_speed;
}

// Called every frame
void ARotatingBlades::Tick(float delta_time)
{
	UWorld *const world = GetWorld();
	if (world == nullptr)
	{
		return;
	}
	if (_entity == nullptr)
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
	spawnEntity(world);
	moveEntities(delta_time);
}

void ARotatingBlades::spawnEntity(UWorld *const world)
{
	FVector SpawnLocation;
	if (_current_state != LaserType::REVERT)
	{
		if (_elapsed_spawn_time < _spawn_period)
		{
			return;
		}
		SpawnLocation = getEntitySpawnLocation(); // Spawn the box just below the spawner
	}
	else
	{
		if (_elapsed_despawn_time > 0)
		{
			return;
		}
		SpawnLocation = getEntityDespawnLocation(); // Spawn the box just above the despawner
	}

	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding;

	auto new_entity = world->SpawnActor<ABladeEntity>(_entity, SpawnLocation, GetActorRotation(), ActorSpawnParams);

	if (new_entity != nullptr)
	{
		// UE_LOG(LogTemp, Warning, TEXT("spawned at %s"), *new_entity->GetActorLocation().ToString());
		new_entity->setParent(this);
		if (_current_state != LaserType::REVERT)
		{
			_entities.push_back(new_entity);
			_elapsed_spawn_time = 0;
		}
		else
		{
			_entities.push_front(new_entity);
			_elapsed_despawn_time = _spawn_period;
		}
	}
}

void ARotatingBlades::moveEntities(float delta_time)
{
	if (_entities.empty())
	{
		return;
	}

	const auto delta_movement = GetActorRotation().RotateVector({0, 0, _entity_speed * delta_time});

	for (const auto &entity_ptr : _entities)
	{
		const auto delta_rotation = FQuat(entity_ptr->GetActorUpVector(), _entity_rotation * delta_time);
		entity_ptr->move(delta_movement, delta_rotation);
	}

	if (_current_state != LaserType::REVERT)
	{
		if (FVector::Distance(_entities.front()->GetActorLocation(), getEntitySpawnLocation()) > getEntityTravelDistance())
		{
			_entities.front()->Destroy();
			_entities.pop_front();
			_elapsed_despawn_time = 0;
		}
	}
	else
	{
		if (FVector::Distance(_entities.back()->GetActorLocation(), getEntityDespawnLocation()) > getEntityTravelDistance())
		{
			_entities.back()->Destroy();
			_entities.pop_back();
			_elapsed_spawn_time = _spawn_period;
		}
	}
}

void ARotatingBlades::setPause()
{
	_current_state = LaserType::PAUSE;
}

void ARotatingBlades::setReset()
{
	_current_state = LaserType::RESET;
}

void ARotatingBlades::setSpeed()
{
	_current_state = LaserType::SPEED;
}

void ARotatingBlades::setReverse()
{
	_current_state = LaserType::REVERT;
}
