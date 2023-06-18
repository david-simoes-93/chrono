// Fill out your copyright notice in the Description page of Project Settings.

#include "Entities/BoxEntity.h"

// Sets default values
ABoxEntity::ABoxEntity()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABoxEntity::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABoxEntity::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ABoxEntity::isPausable()
{
	return false;
}

void ABoxEntity::setPause(bool pause)
{
}
