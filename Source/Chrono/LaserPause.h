// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// ue
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Math/Vector.h"

// chrono
#include "Modifiers/ModifierTypes.h"

#include "LaserPause.generated.h"

// UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
UCLASS(Blueprintable)
class ALaserPause : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALaserPause();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = TypeOfLaser)
	LaserType _laser_type;

	UPROPERTY(BlueprintReadOnly, Category = TypeOfLaser)
	FVector _laser_end;

	UPROPERTY(BlueprintReadOnly, Category = TypeOfLaser)
	bool _actor_hit;
};
