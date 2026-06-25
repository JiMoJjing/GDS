// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GDSLobbyGameMode.generated.h"

class AGDSPlayerController;

UCLASS(Config = Game)
class GDS_API AGDSLobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGDSLobbyGameMode();

	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;

	void HandleStartRequest(AGDSPlayerController* RequestingController);

private:
	UPROPERTY(Config)
	FString MainMapPath;

	TArray<TWeakObjectPtr<AGDSPlayerController>> ConnectedPlayers;

	bool AreAllPlayersReady() const;
	void RemoveInvalidPlayers();
};
