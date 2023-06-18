// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Pausable.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class CHRONO_API UPausable : public UInterface
{
	GENERATED_BODY()
};

class CHRONO_API IPausable
{
	GENERATED_BODY()

public:
	virtual void setPause(bool pause) = 0;
};
