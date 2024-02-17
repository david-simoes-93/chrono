// Fill out your copyright notice in the Description page of Project Settings.

#include "Entities/FragileBox.h"

#include <algorithm>
#include <string>

#include "Engine/StaticMeshActor.h"
#include "Components/BoxComponent.h"

#include "Entities/PistonEntity.h"

// Sets default values
AFragileBox::AFragileBox()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AFragileBox::BeginPlay()
{
	Super::BeginPlay();

	TArray<UActorComponent *> fragments = GetComponentsByTag(UActorComponent::StaticClass(), FName(TEXT("fragment")));
	for (const auto &f : fragments)
	{
		_fragments_static.push_back(f);
	}
	this->SetActorTickEnabled(false);
}

void AFragileBox::OnFragmentation()
{
	if (!_fragments.empty())
	{
		return;
	}

	UWorld *const world = GetWorld();
	if (world == nullptr)
	{
		return;
	}
	if (_static_mesh_entity == nullptr)
	{
		return;
	}

	SetActorEnableCollision(false);
	if (UPrimitiveComponent *PrimComp = Cast<UPrimitiveComponent>(GetRootComponent()))
	{
		PrimComp->SetSimulatePhysics(false);
	}
	_cube_component->SetVisibility(false);

	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	constexpr size_t amt_of_cubes = 10;
	constexpr int XYImpulseMod = 200;
	constexpr int ZImpulseMod = 800;
	constexpr int AngularImpulseMod = 1;
	constexpr int SPAWN_RADIUS = 50;

	// for every fragment
	for (size_t fragment_index = 0; fragment_index < _fragments_static.size(); ++fragment_index)
	{
		auto fragment_inner_component = _fragments_static.at(fragment_index);

		// Spawn a new actor with physics at the location of the fragment
		auto loc = Cast<UPrimitiveComponent>(fragment_inner_component)->GetComponentTransform().GetLocation();
		auto rot = Cast<UPrimitiveComponent>(fragment_inner_component)->GetComponentTransform().Rotator();
		float angle = ((float)fragment_index) * 2 * PI / amt_of_cubes;
		ABoxFragment *new_frag = world->SpawnActor<ABoxFragment>(_static_mesh_entity,
																 loc,
																 rot,
																 ActorSpawnParams);
		if (new_frag == nullptr)
		{
			continue;
		}
		new_frag->setParent(this);

		// Replace mesh
		auto new_mesh = Cast<UStaticMeshComponent>(fragment_inner_component)->GetStaticMesh();
		if (new_mesh == nullptr)
		{
			continue;
		}
		TArray<UStaticMeshComponent *> Components;
		new_frag->GetComponents<UStaticMeshComponent>(Components);
		if (Components.Num() != 1)
		{
			continue;
		}
		UStaticMeshComponent *StaticMeshComponent = Components[0];
		StaticMeshComponent->SetStaticMesh(Cast<UStaticMeshComponent>(fragment_inner_component)->GetStaticMesh());

		// Grab physics component and add impulse
		auto component = new_frag->GetComponentByClass<UPrimitiveComponent>();
		if (component == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("AFragileBox::OnFragmentation: some fragment's UPrimitiveComponent is null"));
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

		_fragments.push_back(new_frag);
	}
}

void AFragileBox::OnAssembly()
{
	if (!_fragment_locations.empty() || !_fragment_rotations.empty() || _fragments_static.size() != _fragments.size())
	{
		return;
	}

	this->SetActorTickEnabled(true);
	_assembly_time_elapsed = 0;

	float max_frag_distance = 0;
	for (AStaticMeshActor *frag : _fragments)
	{
		if (frag == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("AFragileBox::OnAssembly: some fragment is null"));
			continue;
		}
		// save current position of all frags
		_fragment_locations.push_back(frag->GetActorLocation());
		_fragment_rotations.push_back(frag->GetActorRotation());

		// disable physics of all frags
		auto frag_root = Cast<UPrimitiveComponent>(frag->GetRootComponent());
		if (frag_root == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("AFragileBox::OnAssembly: some fragment's UPrimitiveComponent is null"));
			continue;
		}
		frag_root->SetSimulatePhysics(false);

		float distance = (float)FVector::Distance(frag->GetActorLocation(), this->GetActorLocation()) / 100;
		max_frag_distance = std::max(distance / 2, max_frag_distance);
	}
	_assembly_time_max = std::min(ASSEMBLY_TIME_MAX, max_frag_distance);
}

// Called every frame
void AFragileBox::Tick(float delta_time)
{
	Super::Tick(delta_time);
	if (_fragments.empty())
	{
		return;
	}
	_assembly_time_elapsed += delta_time;

	// calculate and set position of fragment in arc given _assembly_time_elapsed
	for (size_t fragment_index = 0; fragment_index < _fragments.size(); ++fragment_index)
	{
		// set location
		FVector start_loc = _fragment_locations.at(fragment_index);
		FVector target_loc = Cast<UPrimitiveComponent>(_fragments_static.at(fragment_index))->GetComponentTransform().GetLocation();

		FVector loc{linear_interpolation(_assembly_time_elapsed, _assembly_time_max, start_loc[0], target_loc[0]),
					linear_interpolation(_assembly_time_elapsed, _assembly_time_max, start_loc[1], target_loc[1]),
					arc_interpolation(_assembly_time_elapsed, _assembly_time_max, start_loc[2], target_loc[2])};

		AStaticMeshActor *frag = _fragments.at(fragment_index);
		if (frag == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("AFragileBox::Tick: some fragment is null"));
			continue;
		}
		frag->SetActorLocation(loc);

		// set rotation
		FRotator start_rot = _fragment_rotations.at(fragment_index);
		FRotator target_rot = FRotator{Cast<UPrimitiveComponent>(_fragments_static.at(fragment_index))->GetComponentTransform().GetRotation()};

		FRotator rot{linear_interpolation(_assembly_time_elapsed, _assembly_time_max, start_rot.Pitch, target_rot.Pitch),
					 linear_interpolation(_assembly_time_elapsed, _assembly_time_max, start_rot.Yaw, target_rot.Yaw),
					 linear_interpolation(_assembly_time_elapsed, _assembly_time_max, start_rot.Roll, target_rot.Roll)};
		frag->SetActorRotation(rot);
	}

	if (_assembly_time_elapsed >= _assembly_time_max)
	{
		reassemble();
	}
}

void AFragileBox::setReset()
{
	_current_state = LaserType::RESET;

	// if we still have fragments, just enable their physics and stop ticking
	if (!_fragments.empty())
	{
		this->SetActorTickEnabled(false);
		for (auto &frag : _fragments)
		{
			Cast<UPrimitiveComponent>(frag->GetRootComponent())->SetSimulatePhysics(true);
		}
		return;
	}

	if (UPrimitiveComponent *PrimComp = Cast<UPrimitiveComponent>(GetRootComponent()))
	{
		PrimComp->SetSimulatePhysics(true);
	}
}

void AFragileBox::setPause()
{
	_current_state = LaserType::PAUSE;

	// if we still have fragments, just disable their physics and stop ticking
	if (!_fragments.empty())
	{
		this->SetActorTickEnabled(false);
		for (auto &frag : _fragments)
		{
			Cast<UPrimitiveComponent>(frag->GetRootComponent())->SetSimulatePhysics(false);
		}
		return;
	}

	if (UPrimitiveComponent *PrimComp = Cast<UPrimitiveComponent>(GetRootComponent()))
	{
		PrimComp->SetSimulatePhysics(false);
	}
}

void AFragileBox::setReverse()
{
	if (_current_state == LaserType::REVERT || _fragments_static.size() != _fragments.size())
	{
		return;
	}

	_current_state = LaserType::REVERT;

	// if we already reverted at some point, clear up the initial locations/rotations of the fragments
	_fragment_locations.clear();
	_fragment_rotations.clear();

	OnAssembly();
}

double AFragileBox::arc_interpolation(double t, double t_max, double h_start, double h_final) const
{
	// if fragment is above target location, linear interpolation
	if (h_start >= h_final)
	{
		return linear_interpolation(t, t_max, h_start, h_final);
	}

	assert(t >= 0);

	// set x in range [0, 10]
	if (t > t_max)
	{
		t = t_max;
	}
	double t_ratio = t / t_max; // [0, 1]
	double x = t_ratio * 10;

	// with x in [0, 10], return h arcing from in [0, 4]
	double a = -0.1;
	double b = 1.4;
	double c = 0;
	double h = a * (x * x) + b * x + c;

	// set h into range [h_start, h_final]
	double h_delta = h_final - h_start;
	return (h * h_delta) / 4 + h_start;
}

double AFragileBox::linear_interpolation(double t, double t_max, double v_start, double v_max) const
{
	assert(t >= 0);

	// set x in range [0, 10]
	if (t > t_max)
	{
		t = t_max;
	}
	double t_ratio = t / t_max; // [0, 1]

	double v_delta = v_max - v_start;
	return v_start + v_delta * t_ratio;
}

void AFragileBox::reassemble()
{
	// despawn them all
	for (size_t fragment_index = 0; fragment_index < _fragments.size(); ++fragment_index)
	{
		_fragments.at(fragment_index)->Destroy();
	}
	_fragments.clear();
	_fragment_locations.clear();
	_fragment_rotations.clear();

	// stop ticking
	this->SetActorTickEnabled(false);

	// make original cube works again
	SetActorEnableCollision(true);

	// if colliding, OnFragmentation()
	TArray<struct FOverlapResult> OutOverlaps;
	UWorld *const world = GetWorld();
	auto box_primitive = Cast<UPrimitiveComponent>(GetRootComponent());
	world->ComponentOverlapMulti(OutOverlaps, box_primitive, this->GetActorLocation(), this->GetActorRotation());
	// box_primitive->GetOverlappingActors(OverlappingActors);
	// box_collider->ComponentOverlapMulti(OutOverlaps, world, this->GetActorLocation(), this->GetActorRotation(), ECollisionChannel::ECC_WorldDynamic);
	for (const auto &overlap : OutOverlaps)
	{
		UE_LOG(LogTemp, Display, TEXT("FragileBox assembly overlapped with %s"), *overlap.GetActor()->GetName());
		if (!overlap.GetActor()->GetClass()->IsChildOf(APistonEntity::StaticClass()))
		{
			continue;
		}

		OnFragmentation();
		_current_state = LaserType::RESET;
		return;
	}

	_cube_component->SetVisibility(true);
	setReset(); // re-enables physics
}