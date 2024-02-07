// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Modifiers/Pausable.h"
#include "Modifiers/Reversible.h"
#include "Modifiers/Speedable.h"

#include "PanelEntity.generated.h"

UCLASS()
class CHRONO_API APanelEntity : public AActor, public IPausable, public IReversible, public ISpeedable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APanelEntity();

	void setParent(AActor *parent);

	void move(const FVector &delta_move, const FVector &world_scale);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	void setPause() override;
	void setReset() override;
	void setSpeed() override;
	void setReverse() override;
	bool isPaused() override
	{
		return _pause_parent != nullptr && _pause_parent->isPaused();
	}
	bool isReset() override
	{
		return _reset_parent != nullptr && _reset_parent->isReset();
	}
	bool isSpeeded() override
	{
		return _speed_parent != nullptr && _speed_parent->isSpeeded();
	}
	bool isReversed() override
	{
		return _reverse_parent != nullptr && _reverse_parent->isReversed();
	}

	IPausable *_pause_parent;
	IResettable *_reset_parent;
	IReversible *_reverse_parent;
	ISpeedable *_speed_parent;
};
