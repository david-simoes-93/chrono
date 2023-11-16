// Fill out your copyright notice in the Description page of Project Settings.

#include "Entities/FragileBox.h"

// Sets default values
AFragileBox::AFragileBox()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AFragileBox::BeginPlay()
{
	Super::BeginPlay();
}

void AFragileBox::OnFragmentation()
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

	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	constexpr size_t amt_of_cubes = 10;
	constexpr int XYImpulseMod = 200;
	constexpr int ZImpulseMod = 800;
	constexpr int AngularImpulseMod = 5;
	constexpr int SPAWN_RADIUS = 50;
	for (size_t fragment_index = 0; fragment_index < 10; ++fragment_index)
	{
		float angle = ((float)fragment_index) * 2 * PI / amt_of_cubes;
		ABoxFragment *new_box = world->SpawnActor<ABoxFragment>(_box_entity,
																GetActorLocation() + FVector{SPAWN_RADIUS * sin(angle),
																							 SPAWN_RADIUS * cos(angle),
																							 SPAWN_RADIUS * 2 * _rand() - SPAWN_RADIUS},
																GetActorRotation(),
																ActorSpawnParams);
		if (new_box != nullptr)
		{
			auto component = new_box->GetComponentByClass<UPrimitiveComponent>();
			if (component)
			{
				UE_LOG(LogTemp, Warning, TEXT("sped %f %f %f"), _rand(), _rand(), 1000 * (1 + _rand()));
				component->AddImpulse((1 + _rand()) * FVector{XYImpulseMod * sin(angle),
															  XYImpulseMod * cos(angle),
															  ZImpulseMod * _rand()},
									  FName(), true);
				component->AddAngularImpulseInRadians(FVector(PI * AngularImpulseMod * _rand(),
															  PI * AngularImpulseMod * _rand(),
															  PI * AngularImpulseMod * _rand()),
													  FName(), true);
			}
			_fragments.push_back(new_box);
		}
	}
}

void AFragileBox::setReset()
{
	_current_state = LaserType::RESET;
}

void AFragileBox::setPause()
{
	_current_state = LaserType::PAUSE;
}

void AFragileBox::setReverse()
{
	_current_state = LaserType::REVERT;
}
