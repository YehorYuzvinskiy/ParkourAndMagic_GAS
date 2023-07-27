// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/AttributeSets/PAM_AttributeSetBase.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"

void UPAM_AttributeSetBase::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
    Super::PostGameplayEffectExecute(Data);

    if (Data.EvaluatedData.Attribute == GetHealthAttribute())
    {
        SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
    }
    else if (Data.EvaluatedData.Attribute == GetMaxMoveSpeedAttribute())
    {
        ACharacter* OwningCharacter = Cast<ACharacter>(GetOwningActor());
        UCharacterMovementComponent* CharacterMovement = OwningCharacter ? OwningCharacter->GetCharacterMovement() : nullptr;

        if (CharacterMovement)
        {
            const float MaxSpeed = GetMaxMoveSpeed();
            CharacterMovement->MaxWalkSpeed = MaxSpeed;
        }
    }
}

UFUNCTION()
void UPAM_AttributeSetBase::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UPAM_AttributeSetBase, Health, OldHealth);
}

UFUNCTION()
void UPAM_AttributeSetBase::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UPAM_AttributeSetBase, Health, OldMaxHealth);
}

void UPAM_AttributeSetBase::OnRep_Stamina(const FGameplayAttributeData& OldStamina)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UPAM_AttributeSetBase, Stamina, OldStamina);
}

void UPAM_AttributeSetBase::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UPAM_AttributeSetBase, Stamina, OldMaxStamina);
}

void UPAM_AttributeSetBase::OnRep_MaxMoveSpeed(const FGameplayAttributeData& OldMaxMoveSpeed)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UPAM_AttributeSetBase, MaxMoveSpeed, OldMaxMoveSpeed);
}

void UPAM_AttributeSetBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME_CONDITION_NOTIFY(UPAM_AttributeSetBase, Health, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UPAM_AttributeSetBase, MaxHealth, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UPAM_AttributeSetBase, Stamina, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UPAM_AttributeSetBase, MaxStamina, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UPAM_AttributeSetBase, MaxMoveSpeed, COND_None, REPNOTIFY_Always);
}