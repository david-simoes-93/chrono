// Fill out your copyright notice in the Description page of Project Settings.

#include "Entities/FallingBoxes.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
AFallingBoxes::AFallingBoxes() : _is_paused{false}
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

	if (world->GetTimeSeconds() > _previous_spawn_time + _spawn_period)
	{
		spawnBox(world);
	}

	moveBoxes(DeltaTime);
}

bool AFallingBoxes::isPausable()
{
	return true;
}

void AFallingBoxes::setPause(bool pause)
{
	_is_paused = pause;
}

void AFallingBoxes::spawnBox(UWorld *const world)
{
	const FRotator SpawnRotation = GetActorRotation();
	const FVector SpawnLocation = GetActorLocation() - SpawnRotation.RotateVector(FVector{0, 0, 50});
	// UE_LOG(LogTemp, Warning, TEXT("SpawnLocation at %s"), *SpawnLocation.ToString());

	// Set Spawn Collision Handling Override
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

	// Spawn the projectile at the muzzle
	ABoxEntity *new_box = world->SpawnActor<ABoxEntity>(_box_entity, SpawnLocation, SpawnRotation, ActorSpawnParams);

	if (new_box != nullptr)
	{
		// UE_LOG(LogTemp, Warning, TEXT("spawned at %s"), *new_box->GetActorLocation().ToString());
		_boxes.push_back(new_box);
		_previous_spawn_time = world->GetTimeSeconds();
	}
}

void AFallingBoxes::moveBoxes(float DeltaTimed)
{
	if (_boxes.empty())
	{
		return;
	}

	if (_is_paused)
	{
		return;
	}

	// if pause, return
	const auto delta_movement = GetActorRotation().RotateVector({0, 0, _box_speed * DeltaTimed});
	// UE_LOG(LogTemp, Warning, TEXT("The vector value is: %s"), *delta_movement.ToString());
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