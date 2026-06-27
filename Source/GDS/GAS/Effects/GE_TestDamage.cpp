// Copyright Epic Games, Inc. All Rights Reserved.

#include "GDS/GAS/Effects/GE_TestDamage.h"

#include "GDS/GAS/Attributes/CombatAttributeSet.h"
#include "GDS/GAS/GDSGameplayTags.h"
#include "GameplayEffectComponents/AssetTagsGameplayEffectComponent.h"

UGE_TestDamage::UGE_TestDamage()
{
	RebuildModifiers();
}

void UGE_TestDamage::PostInitProperties()
{
	Super::PostInitProperties();
	RebuildModifiers();
}

float UGE_TestDamage::GetTestDamage() const
{
	return TestDamage;
}

void UGE_TestDamage::RebuildModifiers()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	Modifiers.Reset();

	FSetByCallerFloat SetByCallerMagnitude;
	SetByCallerMagnitude.DataTag = TAG_Data_Damage_Test;

	FGameplayModifierInfo& DamageModifier = Modifiers.AddDefaulted_GetRef();
	DamageModifier.Attribute = UCombatAttributeSet::GetDamageAttribute();
	DamageModifier.ModifierOp = EGameplayModOp::Additive;
	DamageModifier.ModifierMagnitude = FGameplayEffectModifierMagnitude(SetByCallerMagnitude);

	FInheritedTagContainer AssetTags;
	AssetTags.AddTag(TAG_Effect_Damage_Test);
	FindOrAddComponent<UAssetTagsGameplayEffectComponent>().SetAndApplyAssetTagChanges(AssetTags);
}
