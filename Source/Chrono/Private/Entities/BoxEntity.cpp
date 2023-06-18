// Fill out your copyright notice in the Description page of Project Settings.

#include "Entities/BoxEntity.h"

// Sets default values
ABoxEntity::ABoxEntity()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ABoxEntity::BeginPlay()
{
	Super::BeginPlay();
}

void ABoxEntity::setPause(bool pause)
{
	if (!_pause_parent)
	{
		return;
	}
	_pause_parent->setPause(pause);
}

void ABoxEntity::setPausableParent(IPausable *pause_parent)
{
	if (_pause_parent)
	{
		return;
	}
	_pause_parent = pause_parent;
}
