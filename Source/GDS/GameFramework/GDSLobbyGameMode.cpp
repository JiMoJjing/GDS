// Copyright Epic Games, Inc. All Rights Reserved.

#include "GDS/GameFramework/GDSLobbyGameMode.h"

#include "GDS/GameFramework/GDSCharacter.h"
#include "GDS/GameFramework/GDSLobbyGameState.h"
#include "GDS/GameFramework/GDSPlayerController.h"
#include "GDS/GameFramework/GDSPlayerState.h"

AGDSLobbyGameMode::AGDSLobbyGameMode()
{
	GameStateClass = AGDSLobbyGameState::StaticClass();
	PlayerStateClass = AGDSPlayerState::StaticClass();
	PlayerControllerClass = AGDSPlayerController::StaticClass();
	DefaultPawnClass = AGDSCharacter::StaticClass();
	bUseSeamlessTravel = true;
}
