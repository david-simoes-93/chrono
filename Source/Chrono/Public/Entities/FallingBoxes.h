// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// std
#include <deque>

// ue5
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

// chrono
#include "Entities/Pausable.h"
#include "Entities/BoxEntity.h"

#include "FallingBoxes.generated.h"

UCLASS()
class CHRONO_API AFallingBoxes : public AActor, public IPausable
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
	virtual void Tick(float DeltaTime) override;

	void setPause(bool pause) override;

	void spawnBox(UWorld *const world);
	void moveBoxes(float DeltaTime);

	double _previous_spawn_time;
	std::deque<ABoxEntity *> _boxes;
	bool _is_paused;

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
