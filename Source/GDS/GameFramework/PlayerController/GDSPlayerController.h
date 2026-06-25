// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GDSPlayerController.generated.h"

class AGameStateBase;
class AGDSLobbyGameState;
class ULobbyWidget;
class UWorld;

UCLASS()
class GDS_API AGDSPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	void RequestSetReady(bool bNewReady);
	void RequestStartGame();

protected:
	virtual void BeginPlayingState() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void PostSeamlessTravel() override;
	virtual void PreClientTravel(const FString& PendingURL, ETravelType TravelType, bool bIsSeamlessTravel) override;

private:
	UPROPERTY(Transient)
	TObjectPtr<ULobbyWidget> LobbyWidget;

	TWeakObjectPtr<UWorld> GameStateEventWorld;
	FDelegateHandle GameStateSetDelegateHandle;

	void RegisterGameStateSetEvent();
	void UnregisterGameStateSetEvent();
	void HandleGameStateSet(AGameStateBase* GameState);
	void TryCreateLobbyWidget(AGDSLobbyGameState* LobbyGameState);
	void RemoveLobbyWidget();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetReady(bool bNewReady);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRequestStart();
};
