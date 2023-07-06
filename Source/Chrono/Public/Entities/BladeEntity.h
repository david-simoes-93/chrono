// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Modifiers/Pausable.h"
#include "Modifiers/Reversible.h"
#include "Modifiers/Speedable.h"

#include "BladeEntity.generated.h"

UCLASS()
class CHRONO_API ABladeEntity : public AActor, public IPausable, public IReversible, public ISpeedable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABladeEntity();

	void setParent(AActor *parent);

	void move(const FVector &delta_move, const FQuat &delta_rotation);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	void setPause() override;
	void setReset() override;
	void setSpeed() override;
	void setReverse() override;

	IPausable *_pause_parent;
	IResettable *_reset_parent;
	IReversible *_reverse_parent;
	ISpeedable *_speed_parent;

	UPROPERTY(BlueprintReadOnly, Category = Collision)
	bool _moving;
};
