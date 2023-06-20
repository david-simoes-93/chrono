// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// ue
#include "CoreMinimal.h"

// chrono
#include "Entities/Resettable.h"

#include "Speedable.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class CHRONO_API USpeedable : public UInterface
{
	GENERATED_BODY()
};

class CHRONO_API ISpeedable : public IResettable
{
	GENERATED_BODY()

public:
	virtual void setSpeed() = 0;
};