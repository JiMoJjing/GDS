// Copyright Epic Games, Inc. All Rights Reserved.

#include "GDS/GameFramework/Character/GDSCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GDS/GDS.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"

AGDSCharacter::AGDSCharacter()
{
	bUseControllerRotationYaw = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	if (UCharacterMovementComponent* MovementComponent = GetCharacterMovement())
	{
		MovementComponent->bOrientRotationToMovement = false;
	}

	Phase0InputMappingContext = CreateDefaultSubobject<UInputMappingContext>(TEXT("Phase0InputMappingContext"));

	MoveForwardAction = CreateDefaultSubobject<UInputAction>(TEXT("MoveForwardAction"));
	MoveForwardAction->ValueType = EInputActionValueType::Boolean;
	Phase0InputMappingContext->MapKey(MoveForwardAction, EKeys::W);

	MoveBackwardAction = CreateDefaultSubobject<UInputAction>(TEXT("MoveBackwardAction"));
	MoveBackwardAction->ValueType = EInputActionValueType::Boolean;
	Phase0InputMappingContext->MapKey(MoveBackwardAction, EKeys::S);

	MoveRightAction = CreateDefaultSubobject<UInputAction>(TEXT("MoveRightAction"));
	MoveRightAction->ValueType = EInputActionValueType::Boolean;
	Phase0InputMappingContext->MapKey(MoveRightAction, EKeys::D);

	MoveLeftAction = CreateDefaultSubobject<UInputAction>(TEXT("MoveLeftAction"));
	MoveLeftAction->ValueType = EInputActionValueType::Boolean;
	Phase0InputMappingContext->MapKey(MoveLeftAction, EKeys::A);

	LookAction = CreateDefaultSubobject<UInputAction>(TEXT("LookAction"));
	LookAction->ValueType = EInputActionValueType::Axis2D;
	Phase0InputMappingContext->MapKey(LookAction, EKeys::Mouse2D);
}

void AGDSCharacter::BeginPlay()
{
	Super::BeginPlay();

	const TCHAR* NetContext = HasAuthority() ? TEXT("Server") : TEXT("Client");
	UE_LOG(LogGDS, Log, TEXT("[%s] AGDSCharacter 스폰: %s"), NetContext, *GetName());
}

void AGDSCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	RemovePhase0InputMapping();
	Super::EndPlay(EndPlayReason);
}

void AGDSCharacter::PawnClientRestart()
{
	Super::PawnClientRestart();
	AddPhase0InputMapping();
}

void AGDSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!IsValid(EnhancedInputComponent))
	{
		UE_LOG(LogGDS, Error, TEXT("[Client] EnhancedInputComponent가 없어 Phase 0 이동 입력을 바인딩하지 못함"));
		return;
	}

	EnhancedInputComponent->BindAction(MoveForwardAction, ETriggerEvent::Triggered, this, &ThisClass::MoveForward);
	EnhancedInputComponent->BindAction(MoveBackwardAction, ETriggerEvent::Triggered, this, &ThisClass::MoveBackward);
	EnhancedInputComponent->BindAction(MoveRightAction, ETriggerEvent::Triggered, this, &ThisClass::MoveRight);
	EnhancedInputComponent->BindAction(MoveLeftAction, ETriggerEvent::Triggered, this, &ThisClass::MoveLeft);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);
}

void AGDSCharacter::UnPossessed()
{
	RemovePhase0InputMapping();
	Super::UnPossessed();
}

void AGDSCharacter::AddPhase0InputMapping()
{
	const APlayerController* PlayerController = Cast<APlayerController>(Controller);
	const ULocalPlayer* LocalPlayer = IsValid(PlayerController) ? PlayerController->GetLocalPlayer() : nullptr;
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = IsValid(LocalPlayer)
		? LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()
		: nullptr;
	if (!IsValid(InputSubsystem) || !IsValid(Phase0InputMappingContext))
	{
		return;
	}

	InputSubsystem->RemoveMappingContext(Phase0InputMappingContext);
	InputSubsystem->AddMappingContext(Phase0InputMappingContext, 0);
}

void AGDSCharacter::RemovePhase0InputMapping()
{
	const APlayerController* PlayerController = Cast<APlayerController>(Controller);
	const ULocalPlayer* LocalPlayer = IsValid(PlayerController) ? PlayerController->GetLocalPlayer() : nullptr;
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = IsValid(LocalPlayer)
		? LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()
		: nullptr;
	if (IsValid(InputSubsystem) && IsValid(Phase0InputMappingContext))
	{
		InputSubsystem->RemoveMappingContext(Phase0InputMappingContext);
	}
}

void AGDSCharacter::MoveForward(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		AddMovementInput(GetPlanarForwardDirection());
	}
}

void AGDSCharacter::MoveBackward(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		AddMovementInput(GetPlanarForwardDirection(), -1.0f);
	}
}

void AGDSCharacter::MoveRight(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		AddMovementInput(GetPlanarRightDirection());
	}
}

void AGDSCharacter::MoveLeft(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		AddMovementInput(GetPlanarRightDirection(), -1.0f);
	}
}

void AGDSCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxis = Value.Get<FVector2D>();
	AddControllerYawInput(LookAxis.X);
	AddControllerPitchInput(-LookAxis.Y);
}

FVector AGDSCharacter::GetPlanarForwardDirection() const
{
	const FRotator ControlRotation = Controller != nullptr ? Controller->GetControlRotation() : GetActorRotation();
	return FRotationMatrix(FRotator(0.0f, ControlRotation.Yaw, 0.0f)).GetUnitAxis(EAxis::X);
}

FVector AGDSCharacter::GetPlanarRightDirection() const
{
	const FRotator ControlRotation = Controller != nullptr ? Controller->GetControlRotation() : GetActorRotation();
	return FRotationMatrix(FRotator(0.0f, ControlRotation.Yaw, 0.0f)).GetUnitAxis(EAxis::Y);
}
