// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GDSCharacter.generated.h"

class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

UCLASS()
class GDS_API AGDSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AGDSCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
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

	void AddPhase0InputMapping();
	void RemovePhase0InputMapping();
	void MoveForward(const FInputActionValue& Value);
	void MoveBackward(const FInputActionValue& Value);
	void MoveRight(const FInputActionValue& Value);
	void MoveLeft(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	FVector GetPlanarForwardDirection() const;
	FVector GetPlanarRightDirection() const;
};
