// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// ue
#include "CoreMinimal.h"

// chrono
#include "Entities/Resettable.h"

#include "Reversible.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class CHRONO_API UReversible : public UInterface
{
	GENERATED_BODY()
};

class CHRONO_API IReversible
{
	GENERATED_BODY()

public:
	virtual void setReverse() = 0;
};