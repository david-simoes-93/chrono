// Copyright Epic Games, Inc. All Rights Reserved.

#include "TP_PickUpComponent2.h"

UTP_PickUpComponent2::UTP_PickUpComponent2()
{
	// Setup the Sphere Collision
	SphereRadius = 32.f;
}

void UTP_PickUpComponent2::BeginPlay()
{
	Super::BeginPlay();

	// Register our Overlap Event
	OnComponentBeginOverlap.AddDynamic(this, &UTP_PickUpComponent2::OnSphereBeginOverlap);
}

void UTP_PickUpComponent2::OnSphereBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	// Checking if it is a First Person Character overlapping
	AChronoCharacter *Character = Cast<AChronoCharacter>(OtherActor);
	if (Character != nullptr)
	{
		// Notify that the actor is being picked up
		OnPickUp.Broadcast(Character);

		// Unregister from the Overlap Event so it is no longer triggered
		OnComponentBeginOverlap.RemoveAll(this);
	}
}
