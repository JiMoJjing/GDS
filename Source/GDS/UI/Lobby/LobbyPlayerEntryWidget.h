// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyPlayerEntryWidget.generated.h"

class AGDSPlayerState;
class UTextBlock;

UCLASS()
class GDS_API ULobbyPlayerEntryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitializePlayerState(AGDSPlayerState* InPlayerState);

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void NativeDestruct() override;

private:
	UPROPERTY(Transient)
	TObjectPtr<AGDSPlayerState> PlayerState;

	UPROPERTY(Transient)
	TObjectPtr<UTextBlock> PlayerNameText;

	UPROPERTY(Transient)
	TObjectPtr<UTextBlock> RoomOwnerText;

	UPROPERTY(Transient)
	TObjectPtr<UTextBlock> ReadyStateText;

	UFUNCTION()
	void Refresh();

	void UnbindPlayerState();
};
