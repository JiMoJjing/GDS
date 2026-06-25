// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GDSLobbyGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGDSPlayerListChangedSignature);

UCLASS()
class GDS_API AGDSLobbyGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	virtual void AddPlayerState(APlayerState* PlayerState) override;
	virtual void RemovePlayerState(APlayerState* PlayerState) override;

	UPROPERTY(BlueprintAssignable)
	FGDSPlayerListChangedSignature OnPlayerListChanged;
};
