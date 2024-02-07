// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// std
#include <deque>

// ue
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

// chrono
#include "Modifiers/Pausable.h"
#include "Modifiers/Reversible.h"
#include "Modifiers/Speedable.h"
#include "Entities/BoxEntity.h"
#include "Modifiers/ModifierTypes.h"

#include "VerticalBoxes.generated.h"

UCLASS()
class CHRONO_API AVerticalBoxes : public AActor, public IPausable, public IReversible, public ISpeedable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AVerticalBoxes();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float delta_time) override;

	void setPause() override;
	void setReset() override;
	void setSpeed() override;
	void setReverse() override;
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
	bool isReversed() override
	{
		return _current_state == LaserType::REVERT;
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoxMovement)
	TSubclassOf<class ABoxEntity> _box_entity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoxMovement)
	int32 _distance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoxMovement)
	int32 _distance_boxes_from_spawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoxMovement)
	float _spawn_period; // seconds

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoxMovement)
	float _box_speed; // units / second

	// not implemented
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoxMovement)
	float _acceleration; // units / second²

private:
	void spawnBox(UWorld *const world);
	void moveBoxes(float delta_time);

	FVector getBoxSpawnLocation()
	{
		return GetActorLocation() - GetActorRotation().RotateVector(FVector{0, 0, _distance_boxes_from_spawn});
	}
	FVector getBoxDespawnLocation()
	{
		return GetActorLocation() + GetActorRotation().RotateVector(FVector{0, 0, _distance + _distance_boxes_from_spawn});
	}
	int32 getBoxTravelDistance()
	{
		return _distance + _distance_boxes_from_spawn * 2;
	}

	double _elapsed_spawn_time;
	double _elapsed_despawn_time;
	std::deque<ABoxEntity *> _boxes;
	LaserType _current_state;
};
