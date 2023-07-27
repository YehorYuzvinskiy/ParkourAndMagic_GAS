// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystemInterface.h"
#include "PAM_GameTypes.h"
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

protected:

   
    void GiveAbilities();
    void ApplyStartupEffects();

    virtual void PossessedBy(AController* NewController) override;
    virtual void OnRep_PlayerState() override;


    UPROPERTY(EditDefaultsOnly)
    UPAM_AbilitySystemComponentBase* AbilitySystemComponent;

    UPROPERTY(Transient)
    UPAM_AttributeSetBase* AttributeSet;

    /** Called for forwards/backward input */
    void MoveForward(float Value);

    /** Called for side to side input */
    void MoveRight(float Value);

    /**
     * Called via input to turn at a given rate.
     * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
     */
    void TurnAtRate(float Rate);

    /**
     * Called via input to turn look up/down at a given rate.
     * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
     */
    void LookUpAtRate(float Rate);

    /** Handler for when a touch input begins. */
    void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

    /** Handler for when a touch input stops. */
    void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

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

protected:
    UPROPERTY(ReplicatedUsing= OnRep_CharacterData)
    FCharacterData CharacterData;

    UFUNCTION()
    void OnRep_CharacterData();

    virtual void InitFromCharacterData(const FCharacterData& InCharacterData, bool bFromReplication = false);

    UPROPERTY(EditDefaultsOnly)
    class UCharacterDataAsset* CharacterDataAsset;
};
