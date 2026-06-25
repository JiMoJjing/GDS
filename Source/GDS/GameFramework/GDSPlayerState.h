// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "GDSPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGDSPlayerStateChangedSignature);

UCLASS()
class GDS_API AGDSPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void OnRep_PlayerName() override;

	bool IsRoomOwner() const;
	bool IsReady() const;

	UPROPERTY(BlueprintAssignable)
	FGDSPlayerStateChangedSignature OnIdentityChanged;

	UPROPERTY(BlueprintAssignable)
	FGDSPlayerStateChangedSignature OnRoomOwnerChanged;

	UPROPERTY(BlueprintAssignable)
	FGDSPlayerStateChangedSignature OnReadyChanged;

private:
	friend class AGDSLobbyGameMode;
	friend class AGDSPlayerController;

	UPROPERTY(ReplicatedUsing = OnRep_IsRoomOwner)
	bool bIsRoomOwner = false;

	UPROPERTY(ReplicatedUsing = OnRep_IsReady)
	bool bIsReady = false;

	UFUNCTION()
	void OnRep_IsRoomOwner();

	UFUNCTION()
	void OnRep_IsReady();

	void SetRoomOwner(bool bNewRoomOwner);
	void SetReady(bool bNewReady);
};
