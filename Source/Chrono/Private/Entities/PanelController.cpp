// Fill out your copyright notice in the Description page of Project Settings.

#include "Entities/PanelController.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
APanelController::APanelController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APanelController::BeginPlay()
{
	Super::BeginPlay();
	_current_state = LaserType::SPEED;
}

// Called every frame
void APanelController::Tick(float delta_time)
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

	if (_current_state == LaserType::PAUSE || _current_state == LaserType::RESET)
	{
		return;
	}
	else if (_current_state == LaserType::SPEED)
	{
		// do nothing
	}
	else if (_current_state == LaserType::REVERT)
	{
		delta_time *= -1;
	}

	spawnPanel(world);
	movePanels(delta_time);
}

void APanelController::spawnPanel(UWorld *const world)
{
	FVector SpawnLocation;
	if (_current_state != LaserType::SPEED || _boxes.size() >= _amount_of_panels)
	{
		return;
	}

	if (!_boxes.empty() && _boxes.back()->GetActorScale3D().X < 1)
	{
		return;
	}

	_last_spawn_location = getPanelSpawnLocation(); // Spawn the box on the last panel, or just below the spawner

	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding;
	APanelEntity *new_box = world->SpawnActor<APanelEntity>(_box_entity, _last_spawn_location, GetActorRotation(), ActorSpawnParams);

	if (new_box != nullptr)
	{
		// UE_LOG(LogTemp, Warning, TEXT("spawned at %s"), *new_box->GetActorLocation().ToString());
		new_box->setParent(this);
		new_box->SetActorScale3D(FVector{_box_minimum_scale, _box_minimum_scale, 1});
		_boxes.push_back(new_box);
	}
}

void APanelController::movePanels(float delta_time)
{
	if (_boxes.empty())
	{
		// no longer need to tick
		PrimaryActorTick.bCanEverTick = false;
		return;
	}

	const auto delta_movement = GetActorRotation().RotateVector({0, 0, _box_speed * delta_time});
	if (_current_state == LaserType::SPEED)
	{
		if (FVector::Distance(_boxes.back()->GetActorLocation(), _last_spawn_location) < getPanelTravelDistance())
		{
			_boxes.back()->move(delta_movement, FVector{_box_minimum_scale, _box_minimum_scale, 1});
		}
		else if (_boxes.back()->GetActorScale3D().X < 1)
		{
			float new_size = _boxes.back()->GetActorScale3D().X + _box_scale_speed;
			_boxes.back()->move(FVector{0, 0, 0}, FVector{new_size, new_size, 1});
		}
		else
		{
			// all panels in place, no longer need to tick
			PrimaryActorTick.bCanEverTick = false;
		}
	}
	else if (_current_state == LaserType::REVERT)
	{
		if (_boxes.back()->GetActorScale3D().X > _box_minimum_scale)
		{
			float new_size = _boxes.back()->GetActorScale3D().X - _box_scale_speed;
			_boxes.back()->move(FVector{0, 0, 0}, FVector{new_size, new_size, 1});
		}
		else if (FVector::Distance(_boxes.back()->GetActorLocation(), getPanelSpawnLocation()) < getPanelTravelDistance())
		{
			_boxes.back()->move(delta_movement, FVector{_box_minimum_scale, _box_minimum_scale, 1});
		}
		else
		{
			_boxes.back()->Destroy();
			_boxes.pop_back();
			_last_spawn_location = getPanelSpawnLocation();
		}
	}

	for (APanelGearEntity *gear : _gears)
	{
		gear->move(20 * delta_time);
	}
}

void APanelController::setPause()
{
	_current_state = LaserType::PAUSE;
	// no longer need to tick
	PrimaryActorTick.bCanEverTick = false;
}

void APanelController::setReset()
{
	_current_state = LaserType::RESET;
	// no longer need to tick
	PrimaryActorTick.bCanEverTick = false;
}

void APanelController::setSpeed()
{
	_current_state = LaserType::SPEED;
	if (!_boxes.empty())
	{
		_last_spawn_location = getPanelSpawnLocation(_boxes.size() - 1);
	}
	// tick again
	PrimaryActorTick.bCanEverTick = true;
}

void APanelController::setReverse()
{
	_current_state = LaserType::REVERT;
	// tick again
	PrimaryActorTick.bCanEverTick = true;
}

void APanelController::addChildGear(APanelGearEntity *gear)
{
	_gears.push_back(gear);
}