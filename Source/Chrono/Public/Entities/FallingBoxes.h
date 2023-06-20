// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// std
#include <deque>

// ue
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

// chrono
#include "Entities/Pausable.h"
#include "Entities/Reversible.h"
#include "Entities/Speedable.h"
#include "Entities/BoxEntity.h"
#include "Modifiers/ModifierTypes.h"

#include "FallingBoxes.generated.h"

UCLASS()
class CHRONO_API AFallingBoxes : public AActor, public IPausable, public IResettable, public IReversible, public ISpeedable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFallingBoxes();

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

	void spawnBox(UWorld *const world);
	void moveBoxes(float delta_time);

	double _elapsed_spawn_time;
	std::deque<ABoxEntity *> _boxes;
	LaserType _current_state;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoxMovement)
	TSubclassOf<class ABoxEntity> _box_entity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoxMovement)
	int32 _distance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoxMovement)
	float _spawn_period; // seconds

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoxMovement)
	float _box_speed; // units / second

	// not implemented
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BoxMovement)
	float _acceleration; // units / second²
};
