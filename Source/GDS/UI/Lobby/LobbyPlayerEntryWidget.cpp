// Copyright Epic Games, Inc. All Rights Reserved.

#include "GDS/UI/Lobby/LobbyPlayerEntryWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/TextBlock.h"
#include "GDS/GameFramework/PlayerState/GDSPlayerState.h"

void ULobbyPlayerEntryWidget::InitializePlayerState(AGDSPlayerState* InPlayerState)
{
	UnbindPlayerState();
	PlayerState = InPlayerState;

	if (IsValid(PlayerState))
	{
		PlayerState->OnIdentityChanged.AddUniqueDynamic(this, &ThisClass::Refresh);
		PlayerState->OnRoomOwnerChanged.AddUniqueDynamic(this, &ThisClass::Refresh);
		PlayerState->OnReadyChanged.AddUniqueDynamic(this, &ThisClass::Refresh);
	}

	Refresh();
}

TSharedRef<SWidget> ULobbyPlayerEntryWidget::RebuildWidget()
{
	if (WidgetTree->RootWidget == nullptr)
	{
		UHorizontalBox* RootBox = WidgetTree->ConstructWidget<UHorizontalBox>();
		WidgetTree->RootWidget = RootBox;

		PlayerNameText = WidgetTree->ConstructWidget<UTextBlock>();
		RoomOwnerText = WidgetTree->ConstructWidget<UTextBlock>();
		ReadyStateText = WidgetTree->ConstructWidget<UTextBlock>();

		UHorizontalBoxSlot* PlayerNameSlot = RootBox->AddChildToHorizontalBox(PlayerNameText);
		PlayerNameSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
		PlayerNameSlot->SetPadding(FMargin(8.0f, 4.0f));

		UHorizontalBoxSlot* RoomOwnerSlot = RootBox->AddChildToHorizontalBox(RoomOwnerText);
		RoomOwnerSlot->SetSize(FSlateChildSize(ESlateSizeRule::Automatic));
		RoomOwnerSlot->SetPadding(FMargin(8.0f, 4.0f));

		UHorizontalBoxSlot* ReadyStateSlot = RootBox->AddChildToHorizontalBox(ReadyStateText);
		ReadyStateSlot->SetSize(FSlateChildSize(ESlateSizeRule::Automatic));
		ReadyStateSlot->SetPadding(FMargin(8.0f, 4.0f));
	}

	Refresh();
	return Super::RebuildWidget();
}

void ULobbyPlayerEntryWidget::NativeDestruct()
{
	UnbindPlayerState();
	Super::NativeDestruct();
}

void ULobbyPlayerEntryWidget::Refresh()
{
	if (!IsValid(PlayerState) || !IsValid(PlayerNameText) || !IsValid(RoomOwnerText) || !IsValid(ReadyStateText))
	{
		return;
	}

	FString DisplayName = PlayerState->GetPlayerName();
	if (DisplayName.IsEmpty())
	{
		DisplayName = TEXT("Player");
	}
	DisplayName = FString::Printf(TEXT("%s (%d)"), *DisplayName, PlayerState->GetPlayerId());

	PlayerNameText->SetText(FText::FromString(DisplayName));
	RoomOwnerText->SetText(PlayerState->IsRoomOwner()
		? FText::FromString(TEXT("[방장]"))
		: FText::GetEmpty());
	ReadyStateText->SetText(PlayerState->IsReady()
		? FText::FromString(TEXT("레디"))
		: FText::FromString(TEXT("미레디")));
}

void ULobbyPlayerEntryWidget::UnbindPlayerState()
{
	if (!IsValid(PlayerState))
	{
		return;
	}

	PlayerState->OnIdentityChanged.RemoveDynamic(this, &ThisClass::Refresh);
	PlayerState->OnRoomOwnerChanged.RemoveDynamic(this, &ThisClass::Refresh);
	PlayerState->OnReadyChanged.RemoveDynamic(this, &ThisClass::Refresh);
}
