// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealElectiveV3GameMode.h"
#include "UnrealElectiveV3PlayerController.h"
#include "UnrealElectiveV3Character.h"
#include "UObject/ConstructorHelpers.h"

AUnrealElectiveV3GameMode::AUnrealElectiveV3GameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AUnrealElectiveV3PlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = nullptr;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownPlayerController"));
	if(PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}