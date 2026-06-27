// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "CombatAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS(Config = Game)
class GDS_API UCombatAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UCombatAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Combat")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, Health)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Combat")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, MaxHealth)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Shield, Category = "Combat")
	FGameplayAttributeData Shield;
	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, Shield)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Armor, Category = "Combat")
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, Armor)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MovementSpeed, Category = "Combat")
	FGameplayAttributeData MovementSpeed;
	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, MovementSpeed)

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, Damage)

private:
	static constexpr float MinMovementSpeed = 100.0f;

	UPROPERTY(EditDefaultsOnly, Config, Category = "Combat|Armor")
	float ArmorFlatDamageReduction = 7.0f;

	UPROPERTY(EditDefaultsOnly, Config, Category = "Combat|Armor")
	float ArmorDamageReductionCapPercent = 0.5f;

	void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const;
	void ClampCurrentAttributes();
	void ExecuteDamageCascade(float RawDamage);

	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth);

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);

	UFUNCTION()
	void OnRep_Shield(const FGameplayAttributeData& OldShield);

	UFUNCTION()
	void OnRep_Armor(const FGameplayAttributeData& OldArmor);

	UFUNCTION()
	void OnRep_MovementSpeed(const FGameplayAttributeData& OldMovementSpeed);
};
