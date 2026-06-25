// Copyright Epic Games, Inc. All Rights Reserved.

#include "GDS/GameFramework/GDSCharacter.h"

#include "GDS/GDS.h"

void AGDSCharacter::BeginPlay()
{
	Super::BeginPlay();

	const TCHAR* NetContext = HasAuthority() ? TEXT("Server") : TEXT("Client");
	UE_LOG(LogGDS, Log, TEXT("[%s] AGDSCharacter 스폰: %s"), NetContext, *GetName());
}
