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
	if (_current_state != LaserType::SPEED || _boxes.size() >= 3)
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
		new_box->SetActorScale3D(FVector{0.8, 1, 1});
		_boxes.push_back(new_box);
	}
}

void APanelController::movePanels(float delta_time)
{
	if (_boxes.empty())
	{
		return;
	}

	const auto delta_movement = GetActorRotation().RotateVector({0, 0, _box_speed * delta_time});
	if (_current_state == LaserType::SPEED)
	{
		if (FVector::Distance(_boxes.back()->GetActorLocation(), _last_spawn_location) < getPanelTravelDistance())
		{
			_boxes.back()->move(delta_movement, FVector{0.8, 1, 1});
		}
		else if (_boxes.back()->GetActorScale3D().X < 1)
		{
			_boxes.back()->move(FVector{0, 0, 0}, FVector{_boxes.back()->GetActorScale3D().X + 0.01, 1, 1});
		}
	}
	else if (_current_state == LaserType::REVERT)
	{
		if (_boxes.back()->GetActorScale3D().X > 0.8)
		{
			_boxes.back()->move(FVector{0, 0, 0}, FVector{_boxes.back()->GetActorScale3D().X - 0.01, 1, 1});
		}
		else if (FVector::Distance(_boxes.back()->GetActorLocation(), getPanelSpawnLocation()) < getPanelTravelDistance())
		{
			_boxes.back()->move(delta_movement, FVector{0.8, 1, 1});
		}
		else
		{
			_boxes.back()->Destroy();
			_boxes.pop_back();
		}
	}
}

void APanelController::setPause()
{
	_current_state = LaserType::PAUSE;
}

void APanelController::setReset()
{
	_current_state = LaserType::RESET;
}

void APanelController::setSpeed()
{
	_current_state = LaserType::SPEED;
}

void APanelController::setReverse()
{
	_current_state = LaserType::REVERT;
}
