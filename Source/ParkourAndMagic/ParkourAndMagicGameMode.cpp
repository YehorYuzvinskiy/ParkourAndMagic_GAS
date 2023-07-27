// Copyright Epic Games, Inc. All Rights Reserved.

#include "ParkourAndMagicGameMode.h"
#include "ParkourAndMagicCharacter.h"
#include "UObject/ConstructorHelpers.h"

AParkourAndMagicGameMode::AParkourAndMagicGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
