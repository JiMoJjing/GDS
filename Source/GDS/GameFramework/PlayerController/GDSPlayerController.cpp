// Copyright Epic Games, Inc. All Rights Reserved.

#include "GDS/GameFramework/PlayerController/GDSPlayerController.h"

#include "GDS/GDS.h"
#include "GDS/GameFramework/GameMode/GDSLobbyGameMode.h"
#include "GDS/GameFramework/GameState/GDSLobbyGameState.h"
#include "GDS/GameFramework/PlayerState/GDSPlayerState.h"
#include "GDS/UI/Lobby/LobbyWidget.h"
#include "Engine/World.h"

void AGDSPlayerController::RequestSetReady(bool bNewReady)
{
	ServerSetReady(bNewReady);
}

void AGDSPlayerController::RequestStartGame()
{
	ServerRequestStart();
}

void AGDSPlayerController::BeginPlayingState()
{
	Super::BeginPlayingState();
	RegisterGameStateSetEvent();
}

void AGDSPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnregisterGameStateSetEvent();
	RemoveLobbyWidget();
	Super::EndPlay(EndPlayReason);
}

void AGDSPlayerController::PostSeamlessTravel()
{
	Super::PostSeamlessTravel();
	RegisterGameStateSetEvent();
}

void AGDSPlayerController::PreClientTravel(const FString& PendingURL, ETravelType TravelType, bool bIsSeamlessTravel)
{
	UnregisterGameStateSetEvent();
	RemoveLobbyWidget();
	Super::PreClientTravel(PendingURL, TravelType, bIsSeamlessTravel);
}

void AGDSPlayerController::RegisterGameStateSetEvent()
{
	if (!IsLocalController())
	{
		return;
	}

	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		return;
	}

	if (GameStateSetDelegateHandle.IsValid() && GameStateEventWorld.Get() == World)
	{
		HandleGameStateSet(World->GetGameState());
		return;
	}

	UnregisterGameStateSetEvent();
	GameStateEventWorld = World;
	GameStateSetDelegateHandle = World->GameStateSetEvent.AddUObject(
		this,
		&ThisClass::HandleGameStateSet);
	HandleGameStateSet(World->GetGameState());
}

void AGDSPlayerController::UnregisterGameStateSetEvent()
{
	UWorld* BoundWorld = GameStateEventWorld.Get();
	if (IsValid(BoundWorld) && GameStateSetDelegateHandle.IsValid())
	{
		BoundWorld->GameStateSetEvent.Remove(GameStateSetDelegateHandle);
	}

	GameStateSetDelegateHandle.Reset();
	GameStateEventWorld.Reset();
}

void AGDSPlayerController::HandleGameStateSet(AGameStateBase* GameState)
{
	AGDSLobbyGameState* LobbyGameState = Cast<AGDSLobbyGameState>(GameState);
	if (IsValid(LobbyGameState))
	{
		TryCreateLobbyWidget(LobbyGameState);
		return;
	}

	RemoveLobbyWidget();
}

void AGDSPlayerController::TryCreateLobbyWidget(AGDSLobbyGameState* LobbyGameState)
{
	if (!IsLocalController() || IsValid(LobbyWidget) || !IsValid(LobbyGameState))
	{
		return;
	}

	LobbyWidget = CreateWidget<ULobbyWidget>(this, ULobbyWidget::StaticClass());
	if (!IsValid(LobbyWidget))
	{
		return;
	}

	LobbyWidget->InitializeLobbyGameState(LobbyGameState);
	LobbyWidget->AddToViewport();
	bShowMouseCursor = true;
	SetInputMode(FInputModeUIOnly());
}

void AGDSPlayerController::RemoveLobbyWidget()
{
	if (IsValid(LobbyWidget))
	{
		LobbyWidget->RemoveFromParent();
		LobbyWidget = nullptr;
	}

	bShowMouseCursor = false;
	SetInputMode(FInputModeGameOnly());
}

bool AGDSPlayerController::ServerSetReady_Validate(bool bNewReady)
{
	return IsValid(GetPlayerState<AGDSPlayerState>());
}

void AGDSPlayerController::ServerSetReady_Implementation(bool bNewReady)
{
	AGDSPlayerState* GDSPlayerState = GetPlayerState<AGDSPlayerState>();
	if (!HasAuthority() || !IsValid(GDSPlayerState))
	{
		return;
	}

	const bool bOldReady = GDSPlayerState->IsReady();
	GDSPlayerState->SetReady(bNewReady);

	UE_LOG(LogGDS, Log, TEXT("[Server] 레디 변경: Player=%s, Ready=%s -> %s"),
		*GDSPlayerState->GetPlayerName(),
		bOldReady ? TEXT("true") : TEXT("false"),
		bNewReady ? TEXT("true") : TEXT("false"));
}

bool AGDSPlayerController::ServerRequestStart_Validate()
{
	return IsValid(GetPlayerState<AGDSPlayerState>());
}

void AGDSPlayerController::ServerRequestStart_Implementation()
{
	AGDSLobbyGameMode* LobbyGameMode = GetWorld() != nullptr ? GetWorld()->GetAuthGameMode<AGDSLobbyGameMode>() : nullptr;
	if (!HasAuthority() || !IsValid(LobbyGameMode))
	{
		return;
	}

	LobbyGameMode->HandleStartRequest(this);
}
