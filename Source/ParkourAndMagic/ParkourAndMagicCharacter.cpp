// Copyright Epic Games, Inc. All Rights Reserved.

#include "ParkourAndMagicCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "DataAssets/CharacterDataAsset.h"
#include "AbilitySystem/AttributeSets/PAM_AttributeSetBase.h"
#include "AbilitySystem/Components/PAM_AbilitySystemComponentBase.h"
#include "Net/UnrealNetwork.h"


#include "ActorComponents/PAM_CharacterMovementComponent.h"
#include "ActorComponents/FootstepsComponent.h"
#include "EnhancedInputComponent.h"
#include "InputTriggers.h"
#include "InputActionValue.h"
#include <EnhancedInputSubsystems.h>

//////////////////////////////////////////////////////////////////////////
// AParkourAndMagicCharacter

AParkourAndMagicCharacter::AParkourAndMagicCharacter(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer.SetDefaultSubobjectClass<UPAM_CharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
    // Set size for collision capsule
    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

    // set our turn rate for input
    TurnRateGamepad = 50.f;

    // Don't rotate when the controller rotates. Let that just affect the camera.
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    // Configure character movement
    GetCharacterMovement()->bOrientRotationToMovement = true;             // Character moves in the direction of input...
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);  // ...at this rotation rate

    // Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
    // instead of recompiling to adjust them
    GetCharacterMovement()->JumpZVelocity = 700.f;
    GetCharacterMovement()->AirControl = 0.35f;
    GetCharacterMovement()->MaxWalkSpeed = 500.f;
    GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
    GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

    PAMCharacterMovementComponent = Cast<UPAM_CharacterMovementComponent>(GetCharacterMovement());

    // Create a camera boom (pulls in towards the player if there is a collision)
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 400.0f;        // The camera follows at this distance behind the character
    CameraBoom->bUsePawnControlRotation = true;  // Rotate the arm based on the controller

    // Create a follow camera
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);  // Attach the camera to the end of the boom and let the
                                                                                 // boom adjust to match the controller orientation
    FollowCamera->bUsePawnControlRotation = false;                               // Camera does not rotate relative to arm

    // Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character)
    // are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

    AbilitySystemComponent = CreateDefaultSubobject<UPAM_AbilitySystemComponentBase>(TEXT("AbilitySystemComponent"));
    AbilitySystemComponent->SetIsReplicated(true);
    AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

    AttributeSet = CreateDefaultSubobject<UPAM_AttributeSetBase>(TEXT("AttributeSet"));

    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxMoveSpeedAttribute())
        .AddUObject(this, &AParkourAndMagicCharacter::OnMaxMovementSpeedChanged);

    FootstepsComponent = CreateDefaultSubobject<UFootstepsComponent>(TEXT("FootstepsComponent"));

    PAMMotionWarpingComponent = CreateDefaultSubobject<UPAM_MotionWarpingComponent>(TEXT("MotionWarpingComponent"));
}

void AParkourAndMagicCharacter::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    if (IsValid(CharacterDataAsset))
    {
        SetCharacterData(CharacterDataAsset->CharacterData);
    }
}

UAbilitySystemComponent* AParkourAndMagicCharacter::GetAbilitySystemComponent() const
{
    return AbilitySystemComponent;
}

bool AParkourAndMagicCharacter::ApplyGameplayEffectToSelf(TSubclassOf<UGameplayEffect> Effect, FGameplayEffectContextHandle OnEffectContext)
{
    if (!Effect.Get()) return false;

    FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(Effect, 1, OnEffectContext);
    if (SpecHandle.IsValid())
    {
        FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

        return ActiveGEHandle.WasSuccessfullyApplied();
    }
    return false;
}

void AParkourAndMagicCharacter::PawnClientRestart()
{
    Super::PawnClientRestart();

    if (APlayerController* PC = Cast<APlayerController>(GetController()))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
                ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
        {
            Subsystem->ClearAllMappings();
            Subsystem->AddMappingContext(DefaultMappingContext, 0);
        }
    }
}

void AParkourAndMagicCharacter::Landed(const FHitResult& Hit)
{
    Super::Landed(Hit);
    if (AbilitySystemComponent)
    {
        AbilitySystemComponent->RemoveActiveEffectsWithTags(InAirTags);
    }
}

void AParkourAndMagicCharacter::OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
    Super::OnStartCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);

    if (!CrouchStateEffect.Get()) return;

    if (AbilitySystemComponent)
    {
        FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
        FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(CrouchStateEffect, 1, EffectContext);

        if (SpecHandle.IsValid())
        {
            FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

            if (!ActiveGEHandle.WasSuccessfullyApplied())
            {
                ABILITY_LOG(Log, TEXT("Ability %s failed to apply crouch effect %s"), *GetName(), *GetNameSafe(CrouchStateEffect));
            }
        }
    }
}

void AParkourAndMagicCharacter::OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
    if (AbilitySystemComponent && CrouchStateEffect.Get())
    {
        AbilitySystemComponent->RemoveActiveGameplayEffectBySourceEffect(CrouchStateEffect, AbilitySystemComponent);
    }
    Super::OnEndCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
}

UPAM_MotionWarpingComponent* AParkourAndMagicCharacter::GetMotionWarpingComponent() const
{
    return PAMMotionWarpingComponent;
}

void AParkourAndMagicCharacter::GiveAbilities()
{
    if (HasAuthority() && AbilitySystemComponent)
    {
        for (auto DefaultAbility : CharacterData.Abilities)
        {
            AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(DefaultAbility));
        }
    }
}

void AParkourAndMagicCharacter::ApplyStartupEffects()
{
    if (GetLocalRole() == ROLE_Authority)
    {
        FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
        EffectContext.AddSourceObject(this);

        for (auto CharacterEffect : CharacterData.Effects)
        {
            ApplyGameplayEffectToSelf(CharacterEffect, EffectContext);
        }
    }
}

void AParkourAndMagicCharacter::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);

    AbilitySystemComponent->InitAbilityActorInfo(this, this);

    ApplyStartupEffects();
    GiveAbilities();
}

void AParkourAndMagicCharacter::OnRep_PlayerState()
{
    Super::OnRep_PlayerState();

    AbilitySystemComponent->InitAbilityActorInfo(this, this);
}
FCharacterData AParkourAndMagicCharacter::GetCharacterData() const
{
    return CharacterData;
}

void AParkourAndMagicCharacter::SetCharacterData(const FCharacterData& InCharacterData)
{
    CharacterData = InCharacterData;

    InitFromCharacterData(CharacterData);
}

UFootstepsComponent* AParkourAndMagicCharacter::GetFootstepsComponent() const
{
    return FootstepsComponent;
}

void AParkourAndMagicCharacter::OnMaxMovementSpeedChanged(const FOnAttributeChangeData& Data)
{
    GetCharacterMovement()->MaxWalkSpeed = Data.NewValue;
}

void AParkourAndMagicCharacter::InitFromCharacterData(const FCharacterData& InCharacterData, bool bFromReplication) {}

void AParkourAndMagicCharacter::OnRep_CharacterData()
{
    InitFromCharacterData(CharacterData, true);
}

void AParkourAndMagicCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AParkourAndMagicCharacter, CharacterData);
}
//////////////////////////////////////////////////////////////////////////
// Input

void AParkourAndMagicCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
    // Set up gameplay key bindings
    check(PlayerInputComponent);

    if (UEnhancedInputComponent* PlayerEnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        if (MoveForwardInputAction)
        {
            PlayerEnhancedInputComponent->BindAction(
                MoveForwardInputAction, ETriggerEvent::Triggered, this, &AParkourAndMagicCharacter::OnMoveForwardAction);
        }
        if (MoveRightInputAction)
        {
            PlayerEnhancedInputComponent->BindAction(
                MoveRightInputAction, ETriggerEvent::Triggered, this, &AParkourAndMagicCharacter::OnMoveRightAction);
        }
        if (TurnInputAction)
        {
            PlayerEnhancedInputComponent->BindAction(
                TurnInputAction, ETriggerEvent::Triggered, this, &AParkourAndMagicCharacter::OnTurnAction);
        }

        if (LookUpInputAction)
        {
            PlayerEnhancedInputComponent->BindAction(
                LookUpInputAction, ETriggerEvent::Triggered, this, &AParkourAndMagicCharacter::OnLookUpAction);
        }
        if (JumpInputAction)
        {
            PlayerEnhancedInputComponent->BindAction(
                JumpInputAction, ETriggerEvent::Started, this, &AParkourAndMagicCharacter::OnJumpActionStarted);
            PlayerEnhancedInputComponent->BindAction(
                JumpInputAction, ETriggerEvent::Completed, this, &AParkourAndMagicCharacter::OnJumpActionEnded);
        }
        if (CrouchInputAction)
        {
            PlayerEnhancedInputComponent->BindAction(
                CrouchInputAction, ETriggerEvent::Triggered, this, &AParkourAndMagicCharacter::OnCrouchActionStarted);
            PlayerEnhancedInputComponent->BindAction(
                CrouchInputAction, ETriggerEvent::Completed, this, &AParkourAndMagicCharacter::OnCrouchActionEnded);
        }

        if (SprintInputAction)
        {
            PlayerEnhancedInputComponent->BindAction(
                SprintInputAction, ETriggerEvent::Started, this, &AParkourAndMagicCharacter::OnSprintActionStarted);
            PlayerEnhancedInputComponent->BindAction(
                SprintInputAction, ETriggerEvent::Completed, this, &AParkourAndMagicCharacter::OnSprintActionEnded);
        }
    }
}

void AParkourAndMagicCharacter::OnMoveForwardAction(const FInputActionValue& Value)
{
    const float Magnitude = Value.GetMagnitude();

    if ((Controller != nullptr) && (Magnitude != 0.0f))
    {
        // find out which way is forward
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        // get forward vector
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        AddMovementInput(Direction, Magnitude);
    }
}

void AParkourAndMagicCharacter::OnMoveRightAction(const FInputActionValue& Value)
{
    const float Magnitude = Value.GetMagnitude();

    if ((Controller != nullptr) && (Magnitude != 0.0f))
    {
        // find out which way is right
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        // get right vector
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
        // add movement in that direction
        AddMovementInput(Direction, Magnitude);
    }
}

void AParkourAndMagicCharacter::OnTurnAction(const FInputActionValue& Value)
{
    AddControllerYawInput(Value.GetMagnitude() * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void AParkourAndMagicCharacter::OnLookUpAction(const FInputActionValue& Value)
{
    AddControllerPitchInput(Value.GetMagnitude() * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void AParkourAndMagicCharacter::OnJumpActionStarted(const FInputActionValue& Value)
{
    PAMCharacterMovementComponent->TryTraversal(AbilitySystemComponent);
}

void AParkourAndMagicCharacter::OnJumpActionEnded(const FInputActionValue& Value)
{
    // StopJumping();
}

void AParkourAndMagicCharacter::OnCrouchActionStarted(const FInputActionValue& Value)
{
    if (AbilitySystemComponent)
    {
        AbilitySystemComponent->TryActivateAbilitiesByTag(CrouchTags, true);
    }
}

void AParkourAndMagicCharacter::OnCrouchActionEnded(const FInputActionValue& Value)
{
    if (AbilitySystemComponent)
    {
        AbilitySystemComponent->CancelAbilities(&CrouchTags);
    }
}

void AParkourAndMagicCharacter::OnSprintActionStarted(const FInputActionValue& Value)
{
    if (AbilitySystemComponent)
    {
        AbilitySystemComponent->TryActivateAbilitiesByTag(SprintTags, true);
    }
}

void AParkourAndMagicCharacter::OnSprintActionEnded(const FInputActionValue& Value)
{
    if (AbilitySystemComponent)
    {
        AbilitySystemComponent->CancelAbilities(&SprintTags);
    }
}
