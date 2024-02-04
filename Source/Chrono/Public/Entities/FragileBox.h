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

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void OnFragmentation();

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void OnAssembly();

	// Called every frame
	virtual void Tick(float delta_time) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	void setReverse() override;
	void setReset() override;
	void setPause() override;

private:
	double _rand()
	{
		return (double)std::rand() / (double)RAND_MAX;
	}

	double arc_interpolation(double t, double t_max, double h_start, double h_final)
	{
		// if fragment is above target location, linear interpolation
		if (h_start >= h_final)
		{
			return linear_interpolation(t, t_max, h_final, h_start);
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

	double linear_interpolation(double t, double t_max, double v_start, double v_max)
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

	LaserType _current_state;
	std::vector<AStaticMeshActor *> _fragments;
	std::vector<UActorComponent *> _fragments_static;
	std::vector<FVector> _fragment_locations;
	std::vector<FRotator> _fragment_rotations;
	float _assembly_time_elapsed;
	static constexpr float ASSEMBLY_TIME_MAX = 5;
};
