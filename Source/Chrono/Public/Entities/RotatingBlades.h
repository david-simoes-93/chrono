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
#include "Entities/BladeEntity.h"
#include "Modifiers/ModifierTypes.h"

#include "RotatingBlades.generated.h"

UCLASS()
class CHRONO_API ARotatingBlades : public AActor, public IPausable, public IReversible, public ISpeedable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARotatingBlades();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void setPause() override;
	void setReset() override;
	void setSpeed() override;
	void setReverse() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EntityMovement)
	TSubclassOf<class ABladeEntity> _entity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EntityMovement)
	int32 _distance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EntityMovement)
	int32 _distance_entities_from_spawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EntityMovement)
	float _spawn_period; // seconds

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EntityMovement)
	float _entity_speed; // units / second

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EntityMovement)
	float _entity_rotation; // degress / second

	// not implemented
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EntityMovement)
	float _acceleration; // units / second²

private:
	void spawnEntity(UWorld *const world);
	void moveEntities(float delta_time);

	FVector getEntitySpawnLocation()
	{
		return GetActorLocation() - GetActorRotation().RotateVector(FVector{0, 0, _distance_entities_from_spawn});
	}
	FVector getEntityDespawnLocation()
	{
		return GetActorLocation() + GetActorRotation().RotateVector(FVector{0, 0, _distance + _distance_entities_from_spawn});
	}
	int32 getEntityTravelDistance()
	{
		return _distance + _distance_entities_from_spawn * 2;
	}

	double _elapsed_spawn_time;
	double _elapsed_despawn_time;
	std::deque<ABladeEntity *> _entities;
	LaserType _current_state;
};
