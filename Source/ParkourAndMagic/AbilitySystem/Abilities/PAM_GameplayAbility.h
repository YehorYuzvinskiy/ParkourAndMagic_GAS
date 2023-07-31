// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "PAM_GameplayAbility.generated.h"

UCLASS()
class PARKOURANDMAGIC_API UPAM_GameplayAbility : public UGameplayAbility
{
    GENERATED_BODY()

public:
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);

    virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled);

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Effects")
    TArray<TSubclassOf<UGameplayEffect>> OngoingEffectsToRemoveOnEnd;

    UPROPERTY(EditDefaultsOnly, Category = "Effects")
    TArray<TSubclassOf<UGameplayEffect>> OngoingEffectsToJustApplyOnStart;

    TArray<FActiveGameplayEffectHandle> RemoveOnEndEffectHandles;

    UFUNCTION(BlueprintCallable, BlueprintPure)
    AParkourAndMagicCharacter* GetCharacterInfoFromActor() const;
};
