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

void ABoxEntity::setPause()
{
	if (!_pause_parent)
	{
		return;
	}
	_pause_parent->setPause();
}

void ABoxEntity::setParent(AActor *parent)
{
	if (_pause_parent)
	{
		return;
	}
	_pause_parent = Cast<IPausable>(parent);
	_reset_parent = Cast<IResettable>(parent);
}

void ABoxEntity::setReset()
{
	if (!_reset_parent)
	{
		return;
	}
	_reset_parent->setReset();
}
