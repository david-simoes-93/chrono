// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Modifiers/Pausable.h"
#include "Modifiers/Speedable.h"

#include "PistonEntity.generated.h"

UCLASS()
class CHRONO_API APistonEntity : public AActor, public IPausable, public ISpeedable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APistonEntity();

	void setParent(AActor *parent);

	void move(const FVector &delta_move);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	void setPause() override;
	void setReset() override;
	void setSpeed() override;

	IPausable *_pause_parent;
	IResettable *_reset_parent;
	ISpeedable *_speed_parent;

private:
	AActor *_last_player_collision;
	FVector _last_player_thrust;
};
