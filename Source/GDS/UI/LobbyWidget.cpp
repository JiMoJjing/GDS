// Copyright Epic Games, Inc. All Rights Reserved.

#include "GDS/UI/LobbyWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "GDS/GameFramework/GDSLobbyGameState.h"
#include "GDS/GameFramework/GDSPlayerController.h"
#include "GDS/GameFramework/GDSPlayerState.h"
#include "GDS/UI/LobbyPlayerEntryWidget.h"

void ULobbyWidget::InitializeLobbyGameState(AGDSLobbyGameState* InLobbyGameState)
{
	if (LobbyGameState == InLobbyGameState)
	{
		return;
	}

	if (IsValid(LobbyGameState))
	{
		LobbyGameState->OnPlayerListChanged.RemoveDynamic(this, &ThisClass::RefreshPlayerList);
	}

	LobbyGameState = InLobbyGameState;
	if (IsValid(LobbyGameState))
	{
		LobbyGameState->OnPlayerListChanged.AddUniqueDynamic(this, &ThisClass::RefreshPlayerList);
	}
}

TSharedRef<SWidget> ULobbyWidget::RebuildWidget()
{
	if (WidgetTree->RootWidget == nullptr)
	{
		UVerticalBox* RootBox = WidgetTree->ConstructWidget<UVerticalBox>();
		WidgetTree->RootWidget = RootBox;

		UTextBlock* TitleText = WidgetTree->ConstructWidget<UTextBlock>();
		TitleText->SetText(FText::FromString(TEXT("로비")));
		UVerticalBoxSlot* TitleSlot = RootBox->AddChildToVerticalBox(TitleText);
		TitleSlot->SetPadding(FMargin(16.0f, 12.0f));
		TitleSlot->SetHorizontalAlignment(HAlign_Center);

		PlayerListBox = WidgetTree->ConstructWidget<UVerticalBox>();
		UVerticalBoxSlot* PlayerListSlot = RootBox->AddChildToVerticalBox(PlayerListBox);
		PlayerListSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
		PlayerListSlot->SetPadding(FMargin(16.0f, 8.0f));

		ReadyButton = WidgetTree->ConstructWidget<UButton>();
		ReadyButtonText = WidgetTree->ConstructWidget<UTextBlock>();
		ReadyButtonText->SetText(FText::FromString(TEXT("레디")));
		ReadyButton->AddChild(ReadyButtonText);
		ReadyButton->OnClicked.AddUniqueDynamic(this, &ThisClass::HandleReadyClicked);
		UVerticalBoxSlot* ReadyButtonSlot = RootBox->AddChildToVerticalBox(ReadyButton);
		ReadyButtonSlot->SetPadding(FMargin(16.0f, 8.0f));

		StartButton = WidgetTree->ConstructWidget<UButton>();
		UTextBlock* StartButtonText = WidgetTree->ConstructWidget<UTextBlock>();
		StartButtonText->SetText(FText::FromString(TEXT("시작")));
		StartButton->AddChild(StartButtonText);
		StartButton->OnClicked.AddUniqueDynamic(this, &ThisClass::HandleStartClicked);
		UVerticalBoxSlot* StartButtonSlot = RootBox->AddChildToVerticalBox(StartButton);
		StartButtonSlot->SetPadding(FMargin(16.0f, 8.0f, 16.0f, 16.0f));
	}

	return Super::RebuildWidget();
}

void ULobbyWidget::NativeConstruct()
{
	Super::NativeConstruct();
	RefreshPlayerList();
}

void ULobbyWidget::NativeDestruct()
{
	UnbindPlayerStates();
	if (IsValid(LobbyGameState))
	{
		LobbyGameState->OnPlayerListChanged.RemoveDynamic(this, &ThisClass::RefreshPlayerList);
	}

	LobbyGameState = nullptr;
	Super::NativeDestruct();
}

void ULobbyWidget::RefreshPlayerList()
{
	UnbindPlayerStates();

	if (!IsValid(PlayerListBox))
	{
		return;
	}

	PlayerListBox->ClearChildren();
	if (!IsValid(LobbyGameState))
	{
		RefreshButtons();
		return;
	}

	for (APlayerState* PlayerState : LobbyGameState->PlayerArray)
	{
		AGDSPlayerState* GDSPlayerState = Cast<AGDSPlayerState>(PlayerState);
		if (!IsValid(GDSPlayerState))
		{
			continue;
		}

		GDSPlayerState->OnIdentityChanged.AddUniqueDynamic(this, &ThisClass::HandlePlayerStateChanged);
		GDSPlayerState->OnRoomOwnerChanged.AddUniqueDynamic(this, &ThisClass::HandlePlayerStateChanged);
		GDSPlayerState->OnReadyChanged.AddUniqueDynamic(this, &ThisClass::HandlePlayerStateChanged);
		BoundPlayerStates.Add(GDSPlayerState);

		ULobbyPlayerEntryWidget* EntryWidget = CreateWidget<ULobbyPlayerEntryWidget>(
			GetOwningPlayer(),
			ULobbyPlayerEntryWidget::StaticClass());
		if (IsValid(EntryWidget))
		{
			EntryWidget->InitializePlayerState(GDSPlayerState);
			PlayerListBox->AddChildToVerticalBox(EntryWidget);
		}
	}

	RefreshButtons();
}

void ULobbyWidget::HandlePlayerStateChanged()
{
	RefreshButtons();
}

void ULobbyWidget::HandleReadyClicked()
{
	AGDSPlayerController* PlayerController = Cast<AGDSPlayerController>(GetOwningPlayer());
	AGDSPlayerState* LocalPlayerState = IsValid(PlayerController)
		? PlayerController->GetPlayerState<AGDSPlayerState>()
		: nullptr;
	if (!IsValid(PlayerController) || !IsValid(LocalPlayerState))
	{
		return;
	}

	PlayerController->RequestSetReady(!LocalPlayerState->IsReady());
}

void ULobbyWidget::HandleStartClicked()
{
	AGDSPlayerController* PlayerController = Cast<AGDSPlayerController>(GetOwningPlayer());
	if (IsValid(PlayerController))
	{
		PlayerController->RequestStartGame();
	}
}

void ULobbyWidget::UnbindPlayerStates()
{
	for (AGDSPlayerState* PlayerState : BoundPlayerStates)
	{
		if (!IsValid(PlayerState))
		{
			continue;
		}

		PlayerState->OnIdentityChanged.RemoveDynamic(this, &ThisClass::HandlePlayerStateChanged);
		PlayerState->OnRoomOwnerChanged.RemoveDynamic(this, &ThisClass::HandlePlayerStateChanged);
		PlayerState->OnReadyChanged.RemoveDynamic(this, &ThisClass::HandlePlayerStateChanged);
	}

	BoundPlayerStates.Reset();
}

void ULobbyWidget::RefreshButtons()
{
	AGDSPlayerController* PlayerController = Cast<AGDSPlayerController>(GetOwningPlayer());
	AGDSPlayerState* LocalPlayerState = IsValid(PlayerController)
		? PlayerController->GetPlayerState<AGDSPlayerState>()
		: nullptr;

	if (IsValid(ReadyButtonText))
	{
		ReadyButtonText->SetText(IsValid(LocalPlayerState) && LocalPlayerState->IsReady()
			? FText::FromString(TEXT("레디 취소"))
			: FText::FromString(TEXT("레디")));
	}

	if (IsValid(StartButton))
	{
		const bool bCanStart = IsValid(LocalPlayerState)
			&& LocalPlayerState->IsRoomOwner()
			&& AreAllPlayersReady();
		StartButton->SetIsEnabled(bCanStart);
	}
}

bool ULobbyWidget::AreAllPlayersReady() const
{
	if (!IsValid(LobbyGameState) || LobbyGameState->PlayerArray.IsEmpty())
	{
		return false;
	}

	for (const APlayerState* PlayerState : LobbyGameState->PlayerArray)
	{
		const AGDSPlayerState* GDSPlayerState = Cast<AGDSPlayerState>(PlayerState);
		if (!IsValid(GDSPlayerState) || !GDSPlayerState->IsReady())
		{
			return false;
		}
	}

	return true;
}
