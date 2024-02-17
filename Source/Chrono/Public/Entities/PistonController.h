// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

// chrono
#include "Modifiers/Pausable.h"
#include "Modifiers/Speedable.h"
#include "Entities/PistonEntity.h"
#include "Modifiers/ModifierTypes.h"

#include "PistonController.generated.h"

UCLASS()
class CHRONO_API APistonController : public AActor, public IPausable, public ISpeedable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APistonController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void setPause() override;
	void setReset() override;
	void setSpeed() override;
	bool isPaused() override
	{
		return _current_state == LaserType::PAUSE;
	}
	bool isReset() override
	{
		return _current_state == LaserType::RESET;
	}
	bool isSpeeded() override
	{
		return _current_state == LaserType::SPEED;
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PistonMovement)
	TSubclassOf<class APistonEntity> _entity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PistonMovement)
	int32 _distance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PistonMovement)
	int32 _distance_piston_from_spawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PistonMovement)
	float _piston_period; // seconds

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PistonMovement)
	float _entity_forward_speed; // units / second

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PistonMovement)
	float _entity_back_speed; // units / second

private:
	void spawnPiston(UWorld *const world);
	void thrustPiston(float delta_time);

	FVector getPistonSpawnLocation()
	{
		return GetActorLocation() - GetActorRotation().RotateVector(FVector{0, 0, _distance_piston_from_spawn});
	}
	int32 getPistonTravelDistance()
	{
		return _distance + _distance_piston_from_spawn;
	}
	bool isPistonBehindSpawn()
	{
		if (_piston == nullptr)
		{
			return false;
		}
		return GetActorRotation().GetInverse().RotateVector(getPistonSpawnLocation() - _piston->GetActorLocation()).Z >= 0;
	}

	double _elapsed_thrust_time;
	bool _piston_thrusting_forward;
	APistonEntity *_piston;
	LaserType _current_state;
};
