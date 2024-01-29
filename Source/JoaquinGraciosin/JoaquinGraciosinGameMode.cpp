// Copyright Epic Games, Inc. All Rights Reserved.

#include "JoaquinGraciosinGameMode.h"
#include "JoaquinGraciosinCharacter.h"
#include "UObject/ConstructorHelpers.h"

AJoaquinGraciosinGameMode::AJoaquinGraciosinGameMode()
{
	// set default pawn class to our Blueprinted character

	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/JoaquinGraciosin/Blueprints/Player/BP_MyPlayerCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
