// Copyright Epic Games, Inc. All Rights Reserved.

#include "GDS/GameFramework/Character/GDSCharacter.h"

#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GDS/GDS.h"
#include "GDS/GAS/Attributes/CombatAttributeSet.h"
#include "GDS/GAS/Effects/GE_TestDamage.h"
#include "GDS/GAS/Effects/GE_TestSpeedBuff.h"
#include "GDS/GAS/GDSGameplayTags.h"
#include "GDS/GameFramework/PlayerState/GDSPlayerState.h"
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

	TestDamageAction = CreateDefaultSubobject<UInputAction>(TEXT("TestDamageAction"));
	TestDamageAction->ValueType = EInputActionValueType::Boolean;
	Phase0InputMappingContext->MapKey(TestDamageAction, EKeys::One);

	TestSpeedEffectAction = CreateDefaultSubobject<UInputAction>(TEXT("TestSpeedEffectAction"));
	TestSpeedEffectAction->ValueType = EInputActionValueType::Boolean;
	Phase0InputMappingContext->MapKey(TestSpeedEffectAction, EKeys::Two);
}

UAbilitySystemComponent* AGDSCharacter::GetAbilitySystemComponent() const
{
	const AGDSPlayerState* GDSPlayerState = GetPlayerState<AGDSPlayerState>();
	return IsValid(GDSPlayerState) ? GDSPlayerState->GetAbilitySystemComponent() : nullptr;
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

void AGDSCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	InitializeAbilitySystem();
}

void AGDSCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	InitializeAbilitySystem();
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
	EnhancedInputComponent->BindAction(TestDamageAction, ETriggerEvent::Started, this, &ThisClass::ApplyTestDamage);
	EnhancedInputComponent->BindAction(TestSpeedEffectAction, ETriggerEvent::Started, this, &ThisClass::ApplyTestSpeedEffect);
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

void AGDSCharacter::ApplyTestDamage(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		ServerApplyTestDamage();
	}
}

void AGDSCharacter::ApplyTestSpeedEffect(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		ServerApplyTestSpeedEffect();
	}
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

void AGDSCharacter::InitializeAbilitySystem()
{
	AGDSPlayerState* GDSPlayerState = GetPlayerState<AGDSPlayerState>();
	if (!IsValid(GDSPlayerState))
	{
		return;
	}

	UAbilitySystemComponent* AbilitySystemComponent = GDSPlayerState->GetAbilitySystemComponent();
	const UCombatAttributeSet* CombatAttributeSet = GDSPlayerState->GetCombatAttributeSet();
	if (!IsValid(AbilitySystemComponent) || !IsValid(CombatAttributeSet))
	{
		return;
	}

	AbilitySystemComponent->InitAbilityActorInfo(GDSPlayerState, this);
	BindAttributeChangeDelegates(AbilitySystemComponent);
	ApplyMovementSpeedToCharacter(CombatAttributeSet->GetMovementSpeed());

	const TCHAR* NetContext = HasAuthority() ? TEXT("Server") : TEXT("Client");
	UE_LOG(LogGDS, Log,
		TEXT("[%s] ASC Init: Owner=%s, Avatar=%s, Health=%.1f, MaxHealth=%.1f, Shield=%.1f, Armor=%.1f, MovementSpeed=%.1f, Damage=%.1f"),
		NetContext,
		*GetNameSafe(GDSPlayerState),
		*GetNameSafe(this),
		CombatAttributeSet->GetHealth(),
		CombatAttributeSet->GetMaxHealth(),
		CombatAttributeSet->GetShield(),
		CombatAttributeSet->GetArmor(),
		CombatAttributeSet->GetMovementSpeed(),
		CombatAttributeSet->GetDamage());
}

void AGDSCharacter::BindAttributeChangeDelegates(UAbilitySystemComponent* AbilitySystemComponent)
{
	if (!IsValid(AbilitySystemComponent))
	{
		return;
	}

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UCombatAttributeSet::GetMovementSpeedAttribute()).RemoveAll(this);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UCombatAttributeSet::GetMovementSpeedAttribute()).AddUObject(this, &ThisClass::HandleMovementSpeedChanged);
}

void AGDSCharacter::HandleMovementSpeedChanged(const FOnAttributeChangeData& ChangeData)
{
	if (FMath::IsNearlyEqual(ChangeData.OldValue, ChangeData.NewValue))
	{
		return;
	}

	ApplyMovementSpeedToCharacter(ChangeData.NewValue);

	const TCHAR* NetContext = HasAuthority() ? TEXT("Server") : TEXT("Client");
	UE_LOG(LogGDS, Log, TEXT("[%s] MovementSpeed Delegate: %.1f -> %.1f"),
		NetContext,
		ChangeData.OldValue,
		ChangeData.NewValue);
}

void AGDSCharacter::ApplyMovementSpeedToCharacter(float NewMovementSpeed)
{
	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	if (!IsValid(MovementComponent))
	{
		return;
	}

	const float OldMaxWalkSpeed = MovementComponent->MaxWalkSpeed;
	MovementComponent->MaxWalkSpeed = NewMovementSpeed;

	const TCHAR* NetContext = HasAuthority() ? TEXT("Server") : TEXT("Client");
	UE_LOG(LogGDS, Log, TEXT("[%s] MaxWalkSpeed: %.1f -> %.1f"),
		NetContext,
		OldMaxWalkSpeed,
		MovementComponent->MaxWalkSpeed);
}

bool AGDSCharacter::ServerApplyTestDamage_Validate()
{
	const bool bValid = IsValid(GetAbilitySystemComponent());
	UE_LOG(LogGDS, Log, TEXT("[Server] RPC Validate: ServerApplyTestDamage=%s"), bValid ? TEXT("pass") : TEXT("reject"));
	return bValid;
}

void AGDSCharacter::ServerApplyTestDamage_Implementation()
{
	if (!HasAuthority())
	{
		return;
	}

	UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponent();
	const AGDSPlayerState* GDSPlayerState = GetPlayerState<AGDSPlayerState>();
	const UCombatAttributeSet* CombatAttributeSet = IsValid(GDSPlayerState) ? GDSPlayerState->GetCombatAttributeSet() : nullptr;
	if (!IsValid(AbilitySystemComponent) || !IsValid(CombatAttributeSet))
	{
		return;
	}

	const float OldShield = CombatAttributeSet->GetShield();
	const float OldArmor = CombatAttributeSet->GetArmor();
	const float OldHealth = CombatAttributeSet->GetHealth();

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(UGE_TestDamage::StaticClass(), 1.0f, EffectContext);
	if (!SpecHandle.IsValid())
	{
		UE_LOG(LogGDS, Warning, TEXT("[Server] UGE_TestDamage Spec creation failed"));
		return;
	}

	const UGE_TestDamage* TestDamageEffect = GetDefault<UGE_TestDamage>();
	const float TestDamage = IsValid(TestDamageEffect) ? TestDamageEffect->GetTestDamage() : 0.0f;
	SpecHandle.Data->SetSetByCallerMagnitude(TAG_Data_Damage_Test, TestDamage);

	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

	UE_LOG(LogGDS, Log,
		TEXT("[Server] UGE_TestDamage Applied: Damage=%.1f, Shield %.1f -> %.1f, Armor %.1f -> %.1f, Health %.1f -> %.1f"),
		TestDamage,
		OldShield,
		CombatAttributeSet->GetShield(),
		OldArmor,
		CombatAttributeSet->GetArmor(),
		OldHealth,
		CombatAttributeSet->GetHealth());
}

bool AGDSCharacter::ServerApplyTestSpeedEffect_Validate()
{
	const bool bValid = IsValid(GetAbilitySystemComponent());
	UE_LOG(LogGDS, Log, TEXT("[Server] RPC Validate: ServerApplyTestSpeedEffect=%s"), bValid ? TEXT("pass") : TEXT("reject"));
	return bValid;
}

void AGDSCharacter::ServerApplyTestSpeedEffect_Implementation()
{
	if (!HasAuthority())
	{
		return;
	}

	UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponent();
	const AGDSPlayerState* GDSPlayerState = GetPlayerState<AGDSPlayerState>();
	const UCombatAttributeSet* CombatAttributeSet = IsValid(GDSPlayerState) ? GDSPlayerState->GetCombatAttributeSet() : nullptr;
	if (!IsValid(AbilitySystemComponent) || !IsValid(CombatAttributeSet))
	{
		return;
	}

	const float OldMovementSpeed = CombatAttributeSet->GetMovementSpeed();

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(UGE_TestSpeedBuff::StaticClass(), 1.0f, EffectContext);
	if (!SpecHandle.IsValid())
	{
		UE_LOG(LogGDS, Warning, TEXT("[Server] UGE_TestSpeedBuff Spec creation failed"));
		return;
	}

	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

	const UGE_TestSpeedBuff* TestSpeedEffect = GetDefault<UGE_TestSpeedBuff>();
	const float MovementSpeedDelta = IsValid(TestSpeedEffect) ? TestSpeedEffect->GetMovementSpeedDelta() : 0.0f;

	UE_LOG(LogGDS, Log, TEXT("[Server] UGE_TestSpeedBuff Applied: Delta=%.1f, MovementSpeed %.1f -> %.1f"),
		MovementSpeedDelta,
		OldMovementSpeed,
		CombatAttributeSet->GetMovementSpeed());
}
