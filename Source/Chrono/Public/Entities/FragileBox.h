// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// std
#include <deque>

// ue
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

// chrono
#include "Modifiers/Pausable.h"
#include "Modifiers/Reversible.h"
#include "Entities/BoxFragment.h"
#include "Modifiers/ModifierTypes.h"

#include "FragileBox.generated.h"

UCLASS()
class CHRONO_API AFragileBox : public AActor, public IPausable, public IReversible
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFragileBox();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	TSubclassOf<class ABoxFragment> _box_entity;

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void OnFragmentation();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	void setReverse() override;
	void setReset() override;
	void setPause() override;

	LaserType _current_state;
	std::deque<ABoxFragment *> _fragments;
};
