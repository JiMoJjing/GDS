// Copyright Epic Games, Inc. All Rights Reserved.

#include "GDS/GameFramework/GDSPlayerState.h"

#include "Net/UnrealNetwork.h"

void AGDSPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AGDSPlayerState, bIsRoomOwner, COND_None);
	DOREPLIFETIME_CONDITION(AGDSPlayerState, bIsReady, COND_None);
}

bool AGDSPlayerState::IsRoomOwner() const
{
	return bIsRoomOwner;
}

bool AGDSPlayerState::IsReady() const
{
	return bIsReady;
}

void AGDSPlayerState::OnRep_IsRoomOwner()
{
}

void AGDSPlayerState::OnRep_IsReady()
{
}
