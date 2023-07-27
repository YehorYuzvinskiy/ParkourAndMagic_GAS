// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "PAM_AttributeSetBase.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName)                                                                                       \
    GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName)                                                                             \
    GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName)                                                                                           \
    GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName)                                                                                           \
    GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class PARKOURANDMAGIC_API UPAM_AttributeSetBase : public UAttributeSet
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_Health)
    FGameplayAttributeData Health;
    ATTRIBUTE_ACCESSORS(UPAM_AttributeSetBase, Health);

    UPROPERTY(BlueprintReadOnly, Category = "MaxHealth", ReplicatedUsing = OnRep_MaxHealth)
    FGameplayAttributeData MaxHealth;
    ATTRIBUTE_ACCESSORS(UPAM_AttributeSetBase, MaxHealth);

    UPROPERTY(BlueprintReadOnly, Category = "Stamina", ReplicatedUsing = OnRep_Stamina)
    FGameplayAttributeData Stamina;
    ATTRIBUTE_ACCESSORS(UPAM_AttributeSetBase, Stamina);

    UPROPERTY(BlueprintReadOnly, Category = "MaxStamina", ReplicatedUsing = OnRep_MaxStamina)
    FGameplayAttributeData MaxStamina;
    ATTRIBUTE_ACCESSORS(UPAM_AttributeSetBase, MaxStamina);

    UPROPERTY(BlueprintReadOnly, Category = "MoveSpeed", ReplicatedUsing = OnRep_MaxMoveSpeed)
    FGameplayAttributeData MaxMoveSpeed;
    ATTRIBUTE_ACCESSORS(UPAM_AttributeSetBase, MaxMoveSpeed);

protected:
    virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

    UFUNCTION()
    virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);

    UFUNCTION()
    virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);

    UFUNCTION()
    virtual void OnRep_Stamina(const FGameplayAttributeData& OldStamina);

    UFUNCTION()
    virtual void OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina);

    UFUNCTION()
    virtual void OnRep_MaxMoveSpeed(const FGameplayAttributeData& OldMaxMoveSpeed);
};
