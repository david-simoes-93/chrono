// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Modifiers/Pausable.h"
#include "Modifiers/Reversible.h"
#include "Modifiers/Speedable.h"

#include "PanelGearEntity.generated.h"

class APanelController;

UCLASS()
class CHRONO_API APanelGearEntity : public AActor, public IPausable, public IReversible, public ISpeedable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APanelGearEntity();

	void setParent(APanelController *parent);

	void move(const float &delta_yaw);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	void setPause() override;
	void setReset() override;
	void setSpeed() override;
	void setReverse() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PanelControllerInfo)
	APanelController *parent_controller;

	IPausable *_pause_parent;
	IResettable *_reset_parent;
	IReversible *_reverse_parent;
	ISpeedable *_speed_parent;
};
