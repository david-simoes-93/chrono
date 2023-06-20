// Fill out your copyright notice in the Description page of Project Settings.

#include "Entities/FallingBoxes.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
AFallingBoxes::AFallingBoxes() : _elapsed_spawn_time{0} //, _is_paused{false}
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AFallingBoxes::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AFallingBoxes::Tick(float DeltaTime)
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

	Super::Tick(DeltaTime);

	if (_current_state == LaserType::PAUSE)
	{
		return;
	}

	_elapsed_spawn_time += DeltaTime;
	if (_elapsed_spawn_time > _spawn_period)
	{
		spawnBox(world);
	}

	moveBoxes(DeltaTime);
}

void AFallingBoxes::spawnBox(UWorld *const world)
{
	const FRotator SpawnRotation = GetActorRotation();
	const FVector SpawnLocation = GetActorLocation() - SpawnRotation.RotateVector(FVector{0, 0, 50}); // Spawn the box just below the spawner
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding;

	ABoxEntity *new_box = world->SpawnActor<ABoxEntity>(_box_entity, SpawnLocation, SpawnRotation, ActorSpawnParams);

	if (new_box != nullptr)
	{
		// UE_LOG(LogTemp, Warning, TEXT("spawned at %s"), *new_box->GetActorLocation().ToString());
		new_box->setParent(this);
		_boxes.push_back(new_box);
		_elapsed_spawn_time = 0;
	}
}

void AFallingBoxes::moveBoxes(float DeltaTimed)
{
	if (_boxes.empty())
	{
		return;
	}

	// if pause, return
	const auto delta_movement = GetActorRotation().RotateVector({0, 0, _box_speed * DeltaTimed});
	FHitResult sweep_hit_result;

	for (const auto &box_ptr : _boxes)
	{
		box_ptr->SetActorRelativeLocation(box_ptr->GetActorLocation() + delta_movement, true, &sweep_hit_result, ETeleportType::None);
	}

	if (FVector::Distance(_boxes.front()->GetActorLocation(), GetActorLocation()) > _distance + 100)
	{
		_boxes.front()->Destroy();
		_boxes.pop_front();
	}
}

void AFallingBoxes::setPause()
{
	_current_state = LaserType::PAUSE;
}

void AFallingBoxes::setReset()
{
	_current_state = LaserType::RESET;
}

void AFallingBoxes::setSpeed()
{
	_current_state = LaserType::SPEED;
}

void AFallingBoxes::setReverse()
{
	_current_state = LaserType::REVERT;
}
