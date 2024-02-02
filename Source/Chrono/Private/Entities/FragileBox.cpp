// Fill out your copyright notice in the Description page of Project Settings.

#include "Entities/FragileBox.h"

#include <string>

#include "Engine/StaticMeshActor.h"

#include "Entities/BoxShrinkableFragment.h"

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

	TArray<UActorComponent *> fragments = GetComponentsByTag(UActorComponent::StaticClass(), FName(TEXT("fragment")));
	UE_LOG(LogTemp, Warning, TEXT("sizee %d"), fragments.Num());
	for (const auto &f : fragments)
	{
		_fragments2.push_back(Cast<UActorComponent>(f));
		UE_LOG(LogTemp, Warning, TEXT("adding"));
	}
}

void AFragileBox::OnFragmentation()
{
	UE_LOG(LogTemp, Warning, TEXT("OnFragmentation %d"), _fragments2.size());

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
	constexpr int AngularImpulseMod = 1;
	constexpr int SPAWN_RADIUS = 50;

	for (size_t fragment_index = 0; fragment_index < _fragments2.size(); ++fragment_index)
	{
		UE_LOG(LogTemp, Warning, TEXT("attempting %d"), fragment_index);
		auto fragment_inner_component = _fragments2.at(fragment_index);

		auto loc = Cast<UPrimitiveComponent>(fragment_inner_component)->GetComponentTransform().GetLocation();
		auto rot = Cast<UPrimitiveComponent>(fragment_inner_component)->GetComponentTransform().Rotator();

		float angle = ((float)fragment_index) * 2 * PI / amt_of_cubes;
		AStaticMeshActor *new_box = world->SpawnActor<AStaticMeshActor>(_static_mesh_entity, // AStaticMeshActor::StaticClass(),
																		loc,
																		rot,
																		ActorSpawnParams);
		if (new_box == nullptr)
		{
			continue;
		}

		UE_LOG(LogTemp, Warning, TEXT("spawned %d"), fragment_index);
		auto new_mesh = Cast<UStaticMeshComponent>(fragment_inner_component)->GetStaticMesh();
		if (new_mesh == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("NO MESH %d"), fragment_index);
			continue;
		}

		TArray<UStaticMeshComponent *> Components;
		new_box->GetComponents<UStaticMeshComponent>(Components);
		if (Components.Num() != 1)
		{
			UE_LOG(LogTemp, Warning, TEXT("!=1 %d"), fragment_index);
			continue;
		}
		UStaticMeshComponent *StaticMeshComponent = Components[0];
		StaticMeshComponent->SetStaticMesh(Cast<UStaticMeshComponent>(fragment_inner_component)->GetStaticMesh());
		auto component = new_box->GetComponentByClass<UPrimitiveComponent>();
		if (component == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("NO primitive %d"), fragment_index);
			continue;
		}
		component->AddImpulse((1 + _rand()) * FVector{XYImpulseMod * sin(angle),
													  XYImpulseMod * cos(angle),
													  ZImpulseMod * _rand()},
							  FName(), true);
		component->AddAngularImpulseInRadians(FVector(PI * AngularImpulseMod * _rand(),
													  PI * AngularImpulseMod * _rand(),
													  PI * AngularImpulseMod * _rand()),
											  FName(), true);
	}
}

void AFragileBox::OnAssembly()
{
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
