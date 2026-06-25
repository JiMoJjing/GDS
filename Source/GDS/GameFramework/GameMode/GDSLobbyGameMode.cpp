// Copyright Epic Games, Inc. All Rights Reserved.

#include "GDS/GameFramework/GameMode/GDSLobbyGameMode.h"

#include "GDS/GDS.h"
#include "GDS/GameFramework/Character/GDSCharacter.h"
#include "GDS/GameFramework/GameState/GDSLobbyGameState.h"
#include "GDS/GameFramework/PlayerController/GDSPlayerController.h"
#include "GDS/GameFramework/PlayerState/GDSPlayerState.h"

AGDSLobbyGameMode::AGDSLobbyGameMode()
{
	GameStateClass = AGDSLobbyGameState::StaticClass();
	PlayerStateClass = AGDSPlayerState::StaticClass();
	PlayerControllerClass = AGDSPlayerController::StaticClass();
	DefaultPawnClass = AGDSCharacter::StaticClass();
	bUseSeamlessTravel = true;
}

void AGDSLobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (!HasAuthority())
	{
		return;
	}

	AGDSPlayerController* GDSPlayerController = Cast<AGDSPlayerController>(NewPlayer);
	AGDSPlayerState* GDSPlayerState = IsValid(GDSPlayerController)
		? GDSPlayerController->GetPlayerState<AGDSPlayerState>()
		: nullptr;
	if (!IsValid(GDSPlayerController) || !IsValid(GDSPlayerState))
	{
		return;
	}

	RemoveInvalidPlayers();
	ConnectedPlayers.Add(GDSPlayerController);

	const bool bShouldBeRoomOwner = ConnectedPlayers.Num() == 1;
	GDSPlayerState->SetRoomOwner(bShouldBeRoomOwner);

	UE_LOG(LogGDS, Log, TEXT("[Server] 플레이어 접속: Player=%s, Order=%d, IsRoomOwner=%s"),
		*GDSPlayerState->GetPlayerName(),
		ConnectedPlayers.Num() - 1,
		bShouldBeRoomOwner ? TEXT("true") : TEXT("false"));
}

void AGDSLobbyGameMode::Logout(AController* Exiting)
{
	AGDSPlayerController* ExitingPlayerController = Cast<AGDSPlayerController>(Exiting);
	AGDSPlayerState* ExitingPlayerState = IsValid(ExitingPlayerController)
		? ExitingPlayerController->GetPlayerState<AGDSPlayerState>()
		: nullptr;
	const bool bWasRoomOwner = IsValid(ExitingPlayerState) && ExitingPlayerState->IsRoomOwner();
	const FString ExitingPlayerName = IsValid(ExitingPlayerState) ? ExitingPlayerState->GetPlayerName() : TEXT("Unknown");

	ConnectedPlayers.RemoveAll(
		[ExitingPlayerController](const TWeakObjectPtr<AGDSPlayerController>& PlayerController)
		{
			return !PlayerController.IsValid() || PlayerController.Get() == ExitingPlayerController;
		});

	if (HasAuthority() && bWasRoomOwner)
	{
		RemoveInvalidPlayers();
		if (!ConnectedPlayers.IsEmpty())
		{
			AGDSPlayerController* NewRoomOwnerController = ConnectedPlayers[0].Get();
			AGDSPlayerState* NewRoomOwnerState = IsValid(NewRoomOwnerController)
				? NewRoomOwnerController->GetPlayerState<AGDSPlayerState>()
				: nullptr;
			if (IsValid(NewRoomOwnerState))
			{
				NewRoomOwnerState->SetRoomOwner(true);
				UE_LOG(LogGDS, Log, TEXT("[Server] 방장 승계: Previous=%s, New=%s, ReadyMaintained=%s"),
					*ExitingPlayerName,
					*NewRoomOwnerState->GetPlayerName(),
					NewRoomOwnerState->IsReady() ? TEXT("true") : TEXT("false"));
			}
		}
	}

	Super::Logout(Exiting);
}

void AGDSLobbyGameMode::HandleStartRequest(AGDSPlayerController* RequestingController)
{
	if (!HasAuthority() || !IsValid(RequestingController))
	{
		return;
	}

	const AGDSPlayerState* RequestingPlayerState = RequestingController->GetPlayerState<AGDSPlayerState>();
	const bool bIsRoomOwner = IsValid(RequestingPlayerState) && RequestingPlayerState->IsRoomOwner();
	const bool bAllPlayersReady = AreAllPlayersReady();

	UE_LOG(LogGDS, Log, TEXT("[Server] 시작 요청 검증: Player=%s, IsRoomOwner=%s, AllReady=%s"),
		IsValid(RequestingPlayerState) ? *RequestingPlayerState->GetPlayerName() : TEXT("Unknown"),
		bIsRoomOwner ? TEXT("true") : TEXT("false"),
		bAllPlayersReady ? TEXT("true") : TEXT("false"));

	if (!bIsRoomOwner || !bAllPlayersReady || MainMapPath.IsEmpty() || !IsValid(GetWorld()))
	{
		return;
	}

	UE_LOG(LogGDS, Log, TEXT("[Server] Seamless Travel 시작: Destination=%s"), *MainMapPath);
	GetWorld()->ServerTravel(MainMapPath);
}

bool AGDSLobbyGameMode::AreAllPlayersReady() const
{
	if (GameState == nullptr || GameState->PlayerArray.IsEmpty())
	{
		return false;
	}

	for (const APlayerState* PlayerState : GameState->PlayerArray)
	{
		const AGDSPlayerState* GDSPlayerState = Cast<AGDSPlayerState>(PlayerState);
		if (!IsValid(GDSPlayerState) || !GDSPlayerState->IsReady())
		{
			return false;
		}
	}

	return true;
}

void AGDSLobbyGameMode::RemoveInvalidPlayers()
{
	ConnectedPlayers.RemoveAll(
		[](const TWeakObjectPtr<AGDSPlayerController>& PlayerController)
		{
			return !PlayerController.IsValid();
		});
}
