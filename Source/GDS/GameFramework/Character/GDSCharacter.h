// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "AbilitySystemInterface.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GDSCharacter.generated.h"

class UAbilitySystemComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;
struct FOnAttributeChangeData;

UCLASS()
class GDS_API AGDSCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AGDSCharacter();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	virtual void PawnClientRestart() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void UnPossessed() override;

private:
	UPROPERTY(Transient)
	TObjectPtr<UInputMappingContext> Phase0InputMappingContext;

	UPROPERTY(Transient)
	TObjectPtr<UInputAction> MoveForwardAction;

	UPROPERTY(Transient)
	TObjectPtr<UInputAction> MoveBackwardAction;

	UPROPERTY(Transient)
	TObjectPtr<UInputAction> MoveRightAction;

	UPROPERTY(Transient)
	TObjectPtr<UInputAction> MoveLeftAction;

	UPROPERTY(Transient)
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(Transient)
	TObjectPtr<UInputAction> TestDamageAction;

	UPROPERTY(Transient)
	TObjectPtr<UInputAction> TestSpeedEffectAction;

	void AddPhase0InputMapping();
	void RemovePhase0InputMapping();
	void MoveForward(const FInputActionValue& Value);
	void MoveBackward(const FInputActionValue& Value);
	void MoveRight(const FInputActionValue& Value);
	void MoveLeft(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void ApplyTestDamage(const FInputActionValue& Value);
	void ApplyTestSpeedEffect(const FInputActionValue& Value);
	FVector GetPlanarForwardDirection() const;
	FVector GetPlanarRightDirection() const;
	void InitializeAbilitySystem();
	void BindAttributeChangeDelegates(UAbilitySystemComponent* AbilitySystemComponent);
	void HandleMovementSpeedChanged(const FOnAttributeChangeData& ChangeData);
	void ApplyMovementSpeedToCharacter(float NewMovementSpeed);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerApplyTestDamage();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerApplyTestSpeedEffect();
};
