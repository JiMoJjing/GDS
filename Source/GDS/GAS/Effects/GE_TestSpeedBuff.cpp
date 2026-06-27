// Copyright Epic Games, Inc. All Rights Reserved.

#include "GDS/GAS/Effects/GE_TestSpeedBuff.h"

#include "GDS/GAS/Attributes/CombatAttributeSet.h"

UGE_TestSpeedBuff::UGE_TestSpeedBuff()
{
	RebuildModifiers();
}

void UGE_TestSpeedBuff::PostInitProperties()
{
	Super::PostInitProperties();
	RebuildModifiers();
}

float UGE_TestSpeedBuff::GetMovementSpeedDelta() const
{
	return MovementSpeedDelta;
}

void UGE_TestSpeedBuff::RebuildModifiers()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	Modifiers.Reset();

	FGameplayModifierInfo& SpeedModifier = Modifiers.AddDefaulted_GetRef();
	SpeedModifier.Attribute = UCombatAttributeSet::GetMovementSpeedAttribute();
	SpeedModifier.ModifierOp = EGameplayModOp::Additive;
	SpeedModifier.ModifierMagnitude = FGameplayEffectModifierMagnitude(FScalableFloat(MovementSpeedDelta));
}
