// Fill out your copyright notice in the Description page of Project Settings.

#include "Entities/BoxShrinkableFragment.h"

// Sets default values
ABoxShrinkableFragment::ABoxShrinkableFragment()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ABoxShrinkableFragment::BeginPlay()
{
	Super::BeginPlay();
}

void ABoxShrinkableFragment::shrink()
{
	FVector curr_scale = GetActorScale3D();
	if (curr_scale.X > 0.01)
	{
		SetActorScale3D(curr_scale * 0.95);
	}
	else
	{
		// auto component = new_box->GetComponentByClass<UPrimitiveComponent>();
	}
}

void ABoxShrinkableFragment::grow()
{
}

void ABoxShrinkableFragment::setParent(AActor *parent)
{
	if (_reset_parent || _reverse_parent)
	{
		return;
	}
	_reset_parent = Cast<IResettable>(parent);
	_reverse_parent = Cast<IReversible>(parent);
}

void ABoxShrinkableFragment::setReset()
{
	if (!_reset_parent)
	{
		return;
	}
	_reset_parent->setReset();
}

void ABoxShrinkableFragment::setPause()
{
	if (!_pause_parent)
	{
		return;
	}
	_pause_parent->setPause();
}

void ABoxShrinkableFragment::setReverse()
{
	if (!_reverse_parent)
	{
		return;
	}
	_reverse_parent->setReverse();
}