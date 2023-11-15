// Fill out your copyright notice in the Description page of Project Settings.

#include "Entities/FragileBox.h"

// Sets default values
AFragileBox::AFragileBox()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AFragileBox::BeginPlay()
{
	Super::BeginPlay();
}

void AFragileBox::OnFragmentation()
{
	UWorld *const world = GetWorld();
	if (world == nullptr)
	{
		return;
	}
	if (_box_entity == nullptr)
	{
		return;
	}

	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	ABoxFragment *new_box = world->SpawnActor<ABoxFragment>(_box_entity, GetActorLocation() + FVector{0, 0, 100}, GetActorRotation(), ActorSpawnParams);
	// new_box->AddImpulse(FVector{100, 1000, 100});
	// new_box->SetSpeed();
	if (new_box != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("2nd collision detected!1"));
		_fragments.push_back(new_box);
	}
	UE_LOG(LogTemp, Warning, TEXT("2nd collision detected!11"));
}

void AFragileBox::setReset()
{
	_current_state = LaserType::RESET;
}

void AFragileBox::setPause()
{
	_current_state = LaserType::PAUSE;
}

void AFragileBox::setReverse()
{
	_current_state = LaserType::REVERT;
}
