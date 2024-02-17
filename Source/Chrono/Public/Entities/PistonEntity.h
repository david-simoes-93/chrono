// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Modifiers/Pausable.h"
#include "Modifiers/Speedable.h"
#include "FPS/ChronoCharacter.h"
#include "Entities/FragileBox.h"

#include "PistonEntity.generated.h"

UCLASS()
class CHRONO_API APistonEntity : public AActor, public IPausable, public ISpeedable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APistonEntity();

	void setParent(AActor *parent);

	void move(const FVector &speed, const FVector &delta_move);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	void setPause() override;
	void setReset() override;
	void setSpeed() override;
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

private:
	void launchCharacter(AChronoCharacter *character, const FVector &speed, const FVector &delta_move);
	void moveFragileBox(AFragileBox *box, const FVector &delta_move);

	IPausable *_pause_parent;
	IResettable *_reset_parent;
	ISpeedable *_speed_parent;
};
