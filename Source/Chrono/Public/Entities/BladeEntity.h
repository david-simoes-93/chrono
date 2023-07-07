// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Modifiers/Pausable.h"
#include "Modifiers/Reversible.h"
#include "Modifiers/Speedable.h"

#include "BladeEntity.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBladeRotate, float, yaw);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBladeMovementSet, bool, moving);

UCLASS()
class CHRONO_API ABladeEntity : public AActor, public IPausable, public IReversible, public ISpeedable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABladeEntity();

	void setParent(AActor *parent);

	void move(const FVector &delta_move, const float &delta_yaw);

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

	/** Delegate to whom anyone can subscribe to receive this event */
	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnBladeRotate OnBladeRotate;

	/** Delegate to whom anyone can subscribe to receive this event */
	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnBladeMovementSet OnBladeMovementSet;
};
