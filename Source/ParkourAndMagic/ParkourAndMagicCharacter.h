// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystemInterface.h"
#include "PAM_GameTypes.h"
#include "InputActionValue.h"
#include "ParkourAndMagicCharacter.generated.h"

class UPAM_AbilitySystemComponentBase;
class UPAM_AttributeSetBase;

class UGameplayEffect;
class UGameplayAbility;

UCLASS(config = Game)
class AParkourAndMagicCharacter : public ACharacter, public IAbilitySystemInterface
{
    GENERATED_BODY()

    /** Camera boom positioning the camera behind the character */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    class USpringArmComponent* CameraBoom;

    /** Follow camera */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    class UCameraComponent* FollowCamera;

public:
    

    AParkourAndMagicCharacter(const FObjectInitializer& ObjectInitializer);

    virtual void PostInitializeComponents() override;

    /** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input)
    float TurnRateGamepad;

    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

    bool ApplyGameplayEffectToSelf(TSubclassOf<UGameplayEffect> Effect, FGameplayEffectContextHandle OnEffectContext);

    virtual void PawnClientRestart() override;

protected:

   
    void GiveAbilities();
    void ApplyStartupEffects();

    virtual void PossessedBy(AController* NewController) override;
    virtual void OnRep_PlayerState() override;


    UPROPERTY(EditDefaultsOnly)
    UPAM_AbilitySystemComponentBase* AbilitySystemComponent;

    UPROPERTY(Transient)
    UPAM_AttributeSetBase* AttributeSet;


protected:
    // APawn interface
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    // End of APawn interface

public:
    /** Returns CameraBoom subobject **/
    FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
    /** Returns FollowCamera subobject **/
    FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

    UFUNCTION(BlueprintCallable)
    FCharacterData GetCharacterData() const;

    UFUNCTION(BlueprintCallable)
    void SetCharacterData(const FCharacterData& InCharacterData);

    
    class UFootstepsComponent* GetFootstepsComponent() const;

protected:
    UPROPERTY(ReplicatedUsing= OnRep_CharacterData)
    FCharacterData CharacterData;

    UFUNCTION()
    void OnRep_CharacterData();

    virtual void InitFromCharacterData(const FCharacterData& InCharacterData, bool bFromReplication = false);

    UPROPERTY(EditDefaultsOnly)
    class UCharacterDataAsset* CharacterDataAsset;

    UPROPERTY(BlueprintReadOnly)
    class UFootstepsComponent* FootstepsComponent;

    //Enhanced Input

protected:

    UPROPERTY(EditDefaultsOnly)
    class UInputMappingContext* DefaultMappingContext;

    UPROPERTY(EditDefaultsOnly)
    class UInputAction* MoveForwardInputAction;

    UPROPERTY(EditDefaultsOnly)
    class UInputAction* MoveRightInputAction;

    UPROPERTY(EditDefaultsOnly)
    class UInputAction* TurnInputAction;

    UPROPERTY(EditDefaultsOnly)
    class UInputAction* LookUpInputAction;

    UPROPERTY(EditDefaultsOnly)
    class UInputAction* JumpInputAction;

    void OnMoveForwardAction(const FInputActionValue& Value);

    void OnMoveRightAction(const FInputActionValue& Value);

    void OnTurnAction(const FInputActionValue& Value);

    void OnLookUpAction(const FInputActionValue& Value);

    void OnJumpActionStarted(const FInputActionValue& Value);

    void OnJumpActionEnded(const FInputActionValue& Value);


};
