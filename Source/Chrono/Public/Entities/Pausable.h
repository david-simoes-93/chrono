// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// ue
#include "CoreMinimal.h"

// chrono
#include "Entities/Resettable.h"

#include "Pausable.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class CHRONO_API UPausable : public UInterface
{
	GENERATED_BODY()
};

class CHRONO_API IPausable : public IResettable
{
	GENERATED_BODY()

public:
	virtual void setPause() = 0;
};
