// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "Outpost731GameMode.h"
#include "Outpost731HUD.h"
#include "Outpost731Character.h"
#include "UObject/ConstructorHelpers.h"

AOutpost731GameMode::AOutpost731GameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Player/player_pc"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AOutpost731HUD::StaticClass();
}
