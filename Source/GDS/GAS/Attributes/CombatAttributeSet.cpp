// Copyright Epic Games, Inc. All Rights Reserved.

#include "GDS/GAS/Attributes/CombatAttributeSet.h"

#include "GDS/GDS.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

UCombatAttributeSet::UCombatAttributeSet()
{
	InitMaxHealth(100.0f);
	InitHealth(100.0f);
	InitShield(50.0f);
	InitArmor(50.0f);
	InitMovementSpeed(600.0f);
	InitDamage(0.0f);
}

void UCombatAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UCombatAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCombatAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCombatAttributeSet, Shield, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCombatAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCombatAttributeSet, MovementSpeed, COND_None, REPNOTIFY_Always);
}

void UCombatAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	ClampAttribute(Attribute, NewValue);
}

void UCombatAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	ClampCurrentAttributes();
}

void UCombatAttributeSet::ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
	else if (Attribute == GetMaxHealthAttribute())
	{
		NewValue = FMath::Max(0.0f, NewValue);
	}
	else if (Attribute == GetShieldAttribute() || Attribute == GetArmorAttribute())
	{
		NewValue = FMath::Max(0.0f, NewValue);
	}
	else if (Attribute == GetMovementSpeedAttribute())
	{
		NewValue = FMath::Max(MinMovementSpeed, NewValue);
	}
}

void UCombatAttributeSet::ClampCurrentAttributes()
{
	SetMaxHealth(FMath::Max(0.0f, GetMaxHealth()));
	SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
	SetShield(FMath::Max(0.0f, GetShield()));
	SetArmor(FMath::Max(0.0f, GetArmor()));
	SetMovementSpeed(FMath::Max(MinMovementSpeed, GetMovementSpeed()));
}

void UCombatAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCombatAttributeSet, Health, OldHealth);
	UE_LOG(LogGDS, Log, TEXT("[Client] Attribute Rep: Health %.1f -> %.1f"), OldHealth.GetCurrentValue(), GetHealth());
}

void UCombatAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCombatAttributeSet, MaxHealth, OldMaxHealth);
	UE_LOG(LogGDS, Log, TEXT("[Client] Attribute Rep: MaxHealth %.1f -> %.1f"), OldMaxHealth.GetCurrentValue(), GetMaxHealth());
}

void UCombatAttributeSet::OnRep_Shield(const FGameplayAttributeData& OldShield)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCombatAttributeSet, Shield, OldShield);
	UE_LOG(LogGDS, Log, TEXT("[Client] Attribute Rep: Shield %.1f -> %.1f"), OldShield.GetCurrentValue(), GetShield());
}

void UCombatAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCombatAttributeSet, Armor, OldArmor);
	UE_LOG(LogGDS, Log, TEXT("[Client] Attribute Rep: Armor %.1f -> %.1f"), OldArmor.GetCurrentValue(), GetArmor());
}

void UCombatAttributeSet::OnRep_MovementSpeed(const FGameplayAttributeData& OldMovementSpeed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCombatAttributeSet, MovementSpeed, OldMovementSpeed);
	UE_LOG(LogGDS, Log, TEXT("[Client] Attribute Rep: MovementSpeed %.1f -> %.1f"), OldMovementSpeed.GetCurrentValue(), GetMovementSpeed());
}
