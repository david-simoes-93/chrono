// Fill out your copyright notice in the Description page of Project Settings.

#include "LaserPause.h"

// ue5
#include "DrawDebugHelpers.h"

// chrono
#include "Entities/Pausable.h"

// Sets default values
ALaserPause::ALaserPause() : _actor_hit{false}
{
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ALaserPause::BeginPlay()
{
	SetLifeSpan(5); // must be longer than particle lifetime

	FVector start = GetActorLocation();
	FVector forward = GetActorForwardVector();
	_laser_end = start + forward * 10000;
	FHitResult hit;
	// DrawDebugLine(GetWorld(), start, _laser_end, FColor::Red, false, 2.f, 0.f, 10.f);

	// ray trace
	if (GetWorld())
	{
		_actor_hit = GetWorld()->LineTraceSingleByChannel(hit, start, _laser_end, ECC_Pawn, FCollisionQueryParams(), FCollisionResponseParams());

		// if hit, set laser hit point
		if (_actor_hit && hit.GetActor())
		{
			_laser_end = hit.ImpactPoint;

			// if hit pausable entity, pause it
			if (hit.GetActor()->GetClass()->ImplementsInterface(UPausable::StaticClass()))
			{
				if (IPausable *pausable_actor = Cast<IPausable>(hit.GetActor()))
				{
					pausable_actor->setPause(true);
				}
			}
		}
	}

	// Calls blueprint (hides sphere, sets laser end, etc)
	Super::BeginPlay();
}
