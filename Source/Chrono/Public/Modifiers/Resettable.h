// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Resettable.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class CHRONO_API UResettable : public UInterface
{
	GENERATED_BODY()
};

class CHRONO_API IResettable
{
	GENERATED_BODY()

public:
	virtual void setReset() = 0;
	virtual bool isReset() = 0;
};
