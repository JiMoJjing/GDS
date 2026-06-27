// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameplayEffect.h"
#include "GE_TestSpeedBuff.generated.h"

UCLASS(Config = Game)
class GDS_API UGE_TestSpeedBuff : public UGameplayEffect
{
	GENERATED_BODY()

public:
	UGE_TestSpeedBuff();

	virtual void PostInitProperties() override;

	float GetMovementSpeedDelta() const;

private:
	UPROPERTY(EditDefaultsOnly, Config, Category = "Test")
	float MovementSpeedDelta = -300.0f;

	void RebuildModifiers();
};
