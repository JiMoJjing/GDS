// Copyright Epic Games, Inc. All Rights Reserved.

#include "GDS/GameFramework/PlayerState/GDSPlayerState.h"

#include "AbilitySystemComponent.h"
#include "GDS/GDS.h"
#include "GDS/GAS/Attributes/CombatAttributeSet.h"
#include "Net/UnrealNetwork.h"

AGDSPlayerState::AGDSPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	CombatAttributeSet = CreateDefaultSubobject<UCombatAttributeSet>(TEXT("CombatAttributeSet"));
}

void AGDSPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AGDSPlayerState, bIsRoomOwner, COND_None);
	DOREPLIFETIME_CONDITION(AGDSPlayerState, bIsReady, COND_None);
}

void AGDSPlayerState::OnRep_PlayerName()
{
	Super::OnRep_PlayerName();
	OnIdentityChanged.Broadcast();
}

UAbilitySystemComponent* AGDSPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

bool AGDSPlayerState::IsRoomOwner() const
{
	return bIsRoomOwner;
}

bool AGDSPlayerState::IsReady() const
{
	return bIsReady;
}

UCombatAttributeSet* AGDSPlayerState::GetCombatAttributeSet() const
{
	return CombatAttributeSet;
}

void AGDSPlayerState::OnRep_IsRoomOwner()
{
	UE_LOG(LogGDS, Log, TEXT("[Client] 방장 상태 복제: Player=%s, IsRoomOwner=%s"),
		*GetPlayerName(),
		bIsRoomOwner ? TEXT("true") : TEXT("false"));
	OnRoomOwnerChanged.Broadcast();
}

void AGDSPlayerState::OnRep_IsReady()
{
	UE_LOG(LogGDS, Log, TEXT("[Client] 레디 상태 복제: Player=%s, IsReady=%s"),
		*GetPlayerName(),
		bIsReady ? TEXT("true") : TEXT("false"));
	OnReadyChanged.Broadcast();
}

void AGDSPlayerState::SetRoomOwner(bool bNewRoomOwner)
{
	if (!HasAuthority() || bIsRoomOwner == bNewRoomOwner)
	{
		return;
	}

	bIsRoomOwner = bNewRoomOwner;
	ForceNetUpdate();
}

void AGDSPlayerState::SetReady(bool bNewReady)
{
	if (!HasAuthority() || bIsReady == bNewReady)
	{
		return;
	}

	bIsReady = bNewReady;
	ForceNetUpdate();
}
