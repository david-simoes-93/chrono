// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class LaserType : uint8
{
	RESET = 0,
	PAUSE,
	REVERT,
	SPEED
};
