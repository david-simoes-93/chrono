// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Modifiers/Pausable.h"
#include "Modifiers/Reversible.h"

#include "BoxShrinkableFragment.generated.h"

UCLASS()
class CHRONO_API ABoxShrinkableFragment : public AActor, public IReversible, public IPausable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABoxShrinkableFragment();

	void setParent(AActor *parent);

	void shrink();
	void grow();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	void setReverse() override;
	void setReset() override;
	void setPause() override;
	bool isPaused() override
	{
		return _pause_parent != nullptr && _pause_parent->isPaused();
	}
	bool isReset() override
	{
		return _reset_parent != nullptr && _reset_parent->isReset();
	}
	bool isReversed() override
	{
		return _reverse_parent != nullptr && _reverse_parent->isReversed();
	}

	IResettable *_reset_parent;
	IReversible *_reverse_parent;
	IPausable *_pause_parent;
};
