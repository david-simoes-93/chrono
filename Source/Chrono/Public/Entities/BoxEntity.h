// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Entities/Pausable.h"
#include "BoxEntity.generated.h"

UCLASS()
class CHRONO_API ABoxEntity : public AActor, public IPausable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABoxEntity();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void setPause(bool pause) override;
};
