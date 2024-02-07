// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// ue
#include "CoreMinimal.h"

// chrono
#include "Modifiers/Resettable.h"

#include "Speedable.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class CHRONO_API USpeedable : public UResettable
{
	GENERATED_BODY()
};

class CHRONO_API ISpeedable : public IResettable
{
	GENERATED_BODY()

public:
	virtual void setSpeed() = 0;
	virtual bool isSpeeded() = 0;
};