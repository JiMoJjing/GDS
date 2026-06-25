// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidget.generated.h"

class AGDSLobbyGameState;
class AGDSPlayerState;
class UButton;
class UTextBlock;
class UVerticalBox;

UCLASS()
class GDS_API ULobbyWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitializeLobbyGameState(AGDSLobbyGameState* InLobbyGameState);

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	UPROPERTY(Transient)
	TObjectPtr<AGDSLobbyGameState> LobbyGameState;

	UPROPERTY(Transient)
	TObjectPtr<UVerticalBox> PlayerListBox;

	UPROPERTY(Transient)
	TObjectPtr<UButton> ReadyButton;

	UPROPERTY(Transient)
	TObjectPtr<UTextBlock> ReadyButtonText;

	UPROPERTY(Transient)
	TObjectPtr<UButton> StartButton;

	UPROPERTY(Transient)
	TArray<TObjectPtr<AGDSPlayerState>> BoundPlayerStates;

	UFUNCTION()
	void RefreshPlayerList();

	UFUNCTION()
	void HandlePlayerStateChanged();

	UFUNCTION()
	void HandleReadyClicked();

	UFUNCTION()
	void HandleStartClicked();

	void UnbindPlayerStates();
	void RefreshButtons();
	bool AreAllPlayersReady() const;
};
