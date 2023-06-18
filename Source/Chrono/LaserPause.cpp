// Fill out your copyright notice in the Description page of Project Settings.

#include "LaserPause.h"

// ue5
#include "DrawDebugHelpers.h"

// chrono
#include "Entities/Pausable.h"

// Sets default values
ALaserPause::ALaserPause()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	actor_hit = false;
}

// Called when the game starts or when spawned
void ALaserPause::BeginPlay()
{
	SetLifeSpan(5);

	FVector start = GetActorLocation();
	FVector forward = GetActorForwardVector();
	laser_end = start + forward * 10000;
	FHitResult hit;
	// DrawDebugLine(GetWorld(), start, laser_end, FColor::Red, false, 2.f, 0.f, 10.f);

	// ray trace
	if (GetWorld())
	{
		actor_hit = GetWorld()->LineTraceSingleByChannel(hit, start, laser_end, ECC_Pawn, FCollisionQueryParams(), FCollisionResponseParams());

		// if hit, set laser hit system (blue)
		if (actor_hit && hit.GetActor())
		{
			laser_end = hit.ImpactPoint;

			if (hit.GetActor()->GetClass()->ImplementsInterface(UPausable::StaticClass()))
			{
				UE_LOG(LogTemp, Warning, TEXT("can be paused"));
			}
			//  if pausable hit target, call ChronoPause
			/*if (hit.GetActor()->GetClass()->IsChildOf(Pausable::StaticClass()))
			{
			}
			//hit.GetActor()->GetClass()->IsChildOf(Pausable)

			//hit.GetActor()->GetClass()->
			if (auto pausable_actor = dynamic_cast<Pausable *>(hit.GetActor()); pausable_actor != nullptr)
			{
				pausable_actor->setPause(true);
			}*/
		}
	}

	// Calls blueprint (hides sphere, sets laser end, etc)
	Super::BeginPlay();
}

// Called every frame
void ALaserPause::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// end, ensure particle systems remain until exausted
}
