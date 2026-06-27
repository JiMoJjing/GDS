// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameplayEffect.h"
#include "GE_TestDamage.generated.h"

UCLASS(Config = Game)
class GDS_API UGE_TestDamage : public UGameplayEffect
{
	GENERATED_BODY()

public:
	UGE_TestDamage();

	virtual void PostInitProperties() override;

	float GetTestDamage() const;

private:
	UPROPERTY(EditDefaultsOnly, Config, Category = "Test")
	float TestDamage = 40.0f;

	void RebuildModifiers();
};
