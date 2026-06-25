// Copyright Epic Games, Inc. All Rights Reserved.

#include "GDS/GameFramework/GameState/GDSLobbyGameState.h"

void AGDSLobbyGameState::AddPlayerState(APlayerState* PlayerState)
{
	Super::AddPlayerState(PlayerState);
	OnPlayerListChanged.Broadcast();
}

void AGDSLobbyGameState::RemovePlayerState(APlayerState* PlayerState)
{
	Super::RemovePlayerState(PlayerState);
	OnPlayerListChanged.Broadcast();
}
