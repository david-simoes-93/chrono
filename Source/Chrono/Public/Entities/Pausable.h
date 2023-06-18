// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 *
 */
class CHRONO_API Pausable
{
public:
	virtual bool isPausable() = 0;
	virtual void setPause(bool pause) = 0;
};