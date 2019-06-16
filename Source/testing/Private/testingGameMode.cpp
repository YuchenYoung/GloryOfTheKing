// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "testingGameMode.h"
#include "testingPlayerController.h"
#include "testingCharacter.h"
#include "UObject/ConstructorHelpers.h"

AtestingGameMode::AtestingGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AtestingPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}