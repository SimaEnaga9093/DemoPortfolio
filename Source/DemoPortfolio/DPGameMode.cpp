// Copyright Epic Games, Inc. All Rights Reserved.

#include "DPGameMode.h"

#include "DPPlayerController.h"
#include "DemoPortfolioCharacter.h"
#include "UObject/ConstructorHelpers.h"

ADPGameMode::ADPGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ADPPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownPlayerController"));
	if(PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}