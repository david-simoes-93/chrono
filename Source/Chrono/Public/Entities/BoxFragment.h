// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"

#include "Modifiers/Pausable.h"
#include "Modifiers/Reversible.h"

#include "BoxFragment.generated.h"

UCLASS()
class CHRONO_API ABoxFragment : public AStaticMeshActor, public IReversible, public IPausable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABoxFragment();

	void setParent(AActor *parent);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	void setReverse() override;
	void setReset() override;
	void setPause() override;

	IResettable *_reset_parent;
	IReversible *_reverse_parent;
	IPausable *_pause_parent;
};
