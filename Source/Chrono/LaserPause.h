// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Math/Vector.h"

#include "LaserPause.generated.h"

// UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
UCLASS()
class ALaserPause : public AActor
{
	GENERATED_BODY()

public:
	enum class LaserType
	{
		PAUSE = 0,
		REVERT,
		RESET,
		SPEED
	};

	UPROPERTY(BlueprintReadOnly, Category = TypeOfLaser)
	int laser_type;

	UPROPERTY(BlueprintReadOnly, Category = TypeOfLaser)
	FVector laser_end;

	UPROPERTY(BlueprintReadOnly, Category = TypeOfLaser)
	bool actor_hit;

	// Color ...

	// Sets default values for this actor's properties
	ALaserPause();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
