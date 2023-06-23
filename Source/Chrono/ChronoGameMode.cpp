// Copyright Epic Games, Inc. All Rights Reserved.

#include "ChronoGameMode.h"
#include "FPS/ChronoCharacter.h"
#include "UObject/ConstructorHelpers.h"

AChronoGameMode::AChronoGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
