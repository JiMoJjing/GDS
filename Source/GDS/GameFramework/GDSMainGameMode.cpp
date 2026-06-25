// Copyright Epic Games, Inc. All Rights Reserved.

#include "GDS/GameFramework/GDSMainGameMode.h"

#include "GDS/GameFramework/GDSCharacter.h"
#include "GDS/GameFramework/GDSMainGameState.h"
#include "GDS/GameFramework/GDSPlayerController.h"
#include "GDS/GameFramework/GDSPlayerState.h"

AGDSMainGameMode::AGDSMainGameMode()
{
	GameStateClass = AGDSMainGameState::StaticClass();
	PlayerStateClass = AGDSPlayerState::StaticClass();
	PlayerControllerClass = AGDSPlayerController::StaticClass();
	DefaultPawnClass = AGDSCharacter::StaticClass();
}
