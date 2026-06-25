// Copyright Epic Games, Inc. All Rights Reserved.

#include "GDS/GameFramework/GameMode/GDSMainGameMode.h"

#include "GDS/GameFramework/Character/GDSCharacter.h"
#include "GDS/GameFramework/GameState/GDSMainGameState.h"
#include "GDS/GameFramework/PlayerController/GDSPlayerController.h"
#include "GDS/GameFramework/PlayerState/GDSPlayerState.h"

AGDSMainGameMode::AGDSMainGameMode()
{
	GameStateClass = AGDSMainGameState::StaticClass();
	PlayerStateClass = AGDSPlayerState::StaticClass();
	PlayerControllerClass = AGDSPlayerController::StaticClass();
	DefaultPawnClass = AGDSCharacter::StaticClass();
}
