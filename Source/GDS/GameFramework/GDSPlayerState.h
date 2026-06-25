// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "GDSPlayerState.generated.h"

UCLASS()
class GDS_API AGDSPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	bool IsRoomOwner() const;
	bool IsReady() const;

private:
	UPROPERTY(ReplicatedUsing = OnRep_IsRoomOwner)
	bool bIsRoomOwner = false;

	UPROPERTY(ReplicatedUsing = OnRep_IsReady)
	bool bIsReady = false;

	UFUNCTION()
	void OnRep_IsRoomOwner();

	UFUNCTION()
	void OnRep_IsReady();
};
