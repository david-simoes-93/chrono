// Fill out your copyright notice in the Description page of Project Settings.

#include "Entities/BoxFragment.h"

// Sets default values
ABoxFragment::ABoxFragment()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ABoxFragment::BeginPlay()
{
	Super::BeginPlay();
}

void ABoxFragment::setParent(AActor *parent)
{
	if (_reset_parent || _reverse_parent)
	{
		return;
	}
	_reset_parent = Cast<IResettable>(parent);
	_reverse_parent = Cast<IReversible>(parent);
}

void ABoxFragment::setReset()
{
	if (!_reset_parent)
	{
		return;
	}
	_reset_parent->setReset();
}

void ABoxFragment::setPause()
{
	if (!_pause_parent)
	{
		return;
	}
	_pause_parent->setPause();
}

void ABoxFragment::setReverse()
{
	if (!_reverse_parent)
	{
		return;
	}
	_reverse_parent->setReverse();
}
