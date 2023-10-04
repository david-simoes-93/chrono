// Fill out your copyright notice in the Description page of Project Settings.

#include "Entities/PanelGearEntity.h"

#include "Entities/PanelController.h"

// Sets default values
APanelGearEntity::APanelGearEntity()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void APanelGearEntity::BeginPlay()
{
	Super::BeginPlay();
	if (parent_controller)
	{
		setParent(parent_controller);
	}
}

void APanelGearEntity::setParent(APanelController *parent)
{
	if (_pause_parent || _reset_parent || _reverse_parent || _speed_parent)
	{
		return;
	}
	_pause_parent = Cast<IPausable>(parent);
	_reset_parent = Cast<IResettable>(parent);
	_reverse_parent = Cast<IReversible>(parent);
	_speed_parent = Cast<ISpeedable>(parent);
	parent->addChildGear(this);
}

void APanelGearEntity::move(const float &delta_yaw)
{
	AddActorLocalRotation(FRotator{0, delta_yaw, 0});
}

void APanelGearEntity::setPause()
{
	if (!_pause_parent)
	{
		return;
	}
	_pause_parent->setPause();
}

void APanelGearEntity::setReset()
{
	if (!_reset_parent)
	{
		return;
	}
	_reset_parent->setReset();
}

void APanelGearEntity::setSpeed()
{
	if (!_speed_parent)
	{
		return;
	}
	_speed_parent->setSpeed();
}

void APanelGearEntity::setReverse()
{
	if (!_reverse_parent)
	{
		return;
	}
	_reverse_parent->setReverse();
}
