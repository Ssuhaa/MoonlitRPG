// Copyright Epic Games, Inc. All Rights Reserved.

#include "MoonlitRPGGameMode.h"
#include "MoonlitRPGCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMoonlitRPGGameMode::AMoonlitRPGGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
