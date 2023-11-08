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
#include "Entities/PanelEntity.h"
#include "Entities/PanelGearEntity.h"
#include "Modifiers/ModifierTypes.h"

#include "PanelController.generated.h"

UCLASS()
class CHRONO_API APanelController : public AActor, public IPausable, public IReversible, public ISpeedable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APanelController();

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PanelMovement)
	TSubclassOf<class APanelEntity> _box_entity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PanelMovement)
	int32 _distance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PanelMovement)
	int32 _distance_boxes_from_spawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PanelMovement)
	float _box_speed; // units / second

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PanelMovement)
	int32 _amount_of_panels;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PanelMovement)
	float _box_scale_speed; // units / second

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PanelMovement)
	float _box_minimum_scale; // ratio [0, 1]

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PanelMovement)
	int32 _amount_of_starting_panels;

	void addChildGear(APanelGearEntity *gear);

private:
	void spawnPanel(UWorld *const world);
	void spawnInitialPanels();
	void movePanels(float delta_time);

	FVector getPanelSpawnLocation()
	{
		return getPanelSpawnLocation(_boxes.size());
	}

	FVector getPanelSpawnLocation(size_t amount_of_panels)
	{
		// if no panels, spawn first panel behind spawner
		// otherwise spawn it on the last panel's location
		int32 distance_from_spawner = -_distance_boxes_from_spawn + _distance * amount_of_panels;
		return GetActorLocation() + GetActorRotation().RotateVector(FVector{0, 0, distance_from_spawner});
	}

	int32 getPanelTravelDistance()
	{
		return _distance;
	}

	double _elapsed_spawn_time;
	FVector _last_spawn_location;
	std::deque<APanelEntity *> _boxes;
	std::vector<APanelGearEntity *> _gears;
	LaserType _current_state;
};
