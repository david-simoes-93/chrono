// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// std
#include <deque>
#include <vector>
#include <cassert>

// ue
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

// chrono
#include "Modifiers/Pausable.h"
#include "Modifiers/Reversible.h"
#include "Entities/BoxFragment.h"
#include "Modifiers/ModifierTypes.h"

#include "FragileBox.generated.h"

UCLASS()
class CHRONO_API AFragileBox : public AActor, public IPausable, public IReversible
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFragileBox();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	TSubclassOf<class AStaticMeshActor> _static_mesh_entity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	UStaticMeshComponent *_cube_component;

	// Called by a sped-up piston, breaks box in many small fragments
	void OnFragmentation();

	// Called every frame
	virtual void Tick(float delta_time) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	void setReverse() override;
	void setReset() override;
	void setPause() override;
	bool isPaused() override
	{
		return _current_state == LaserType::PAUSE;
	}
	bool isReset() override
	{
		return _current_state == LaserType::RESET;
	}
	bool isReversed() override
	{
		return _current_state == LaserType::REVERT;
	}

private:
	// Called when reversing, after all fragments are back in their place
	void OnAssembly();

	double _rand()
	{
		return (double)std::rand() / (double)RAND_MAX;
	}

	// returns a value v between v_start and v_max based on a current time t out of t_max, where v is linear
	double arc_interpolation(double t, double t_max, double h_start, double h_final) const;

	// returns a value v between v_start and v_max based on a current time t out of t_max, where v makes an arc shape
	double linear_interpolation(double t, double t_max, double v_start, double v_max) const;

	void reassemble();

	LaserType _current_state;
	std::vector<ABoxFragment *> _fragments;
	std::vector<UActorComponent *> _fragments_static;
	std::vector<FVector> _fragment_locations;
	std::vector<FRotator> _fragment_rotations;
	float _assembly_time_elapsed;
	float _assembly_time_max;
	static constexpr float ASSEMBLY_TIME_MAX = 5;
	static constexpr double dASSEMBLY_TIME_MAX = ASSEMBLY_TIME_MAX;
};
