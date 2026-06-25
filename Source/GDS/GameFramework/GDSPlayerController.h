// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GDSPlayerController.generated.h"

class ULobbyWidget;

UCLASS()
class GDS_API AGDSPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	void RequestSetReady(bool bNewReady);
	void RequestStartGame();

protected:
	virtual void BeginPlayingState() override;
	virtual void OnRep_PlayerState() override;
	virtual void PreClientTravel(const FString& PendingURL, ETravelType TravelType, bool bIsSeamlessTravel) override;

private:
	UPROPERTY(Transient)
	TObjectPtr<ULobbyWidget> LobbyWidget;

	void TryCreateLobbyWidget();
	void RemoveLobbyWidget();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetReady(bool bNewReady);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRequestStart();
};
