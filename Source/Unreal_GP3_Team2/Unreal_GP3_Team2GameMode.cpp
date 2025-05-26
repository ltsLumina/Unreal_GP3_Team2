// Copyright Epic Games, Inc. All Rights Reserved.

#include "Unreal_GP3_Team2GameMode.h"
#include "Unreal_GP3_Team2Character.h"
#include "UObject/ConstructorHelpers.h"

AUnreal_GP3_Team2GameMode::AUnreal_GP3_Team2GameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	//DefaultPawnClass = PlayerPawnClassFinder.Class;

}
