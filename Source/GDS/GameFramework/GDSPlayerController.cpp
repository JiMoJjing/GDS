// Copyright Epic Games, Inc. All Rights Reserved.

#include "GDS/GameFramework/GDSPlayerController.h"

#include "GDS/GDS.h"
#include "GDS/GameFramework/GDSLobbyGameMode.h"
#include "GDS/GameFramework/GDSLobbyGameState.h"
#include "GDS/GameFramework/GDSPlayerState.h"
#include "GDS/UI/LobbyWidget.h"

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
	TryCreateLobbyWidget();
}

void AGDSPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	TryCreateLobbyWidget();
}

void AGDSPlayerController::PreClientTravel(const FString& PendingURL, ETravelType TravelType, bool bIsSeamlessTravel)
{
	RemoveLobbyWidget();
	Super::PreClientTravel(PendingURL, TravelType, bIsSeamlessTravel);
}

void AGDSPlayerController::TryCreateLobbyWidget()
{
	if (!IsLocalController() || IsValid(LobbyWidget))
	{
		return;
	}

	if (!IsValid(GetWorld()) || !GetWorld()->GetMapName().EndsWith(TEXT("Lobby")))
	{
		return;
	}

	LobbyWidget = CreateWidget<ULobbyWidget>(this, ULobbyWidget::StaticClass());
	if (!IsValid(LobbyWidget))
	{
		return;
	}

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
