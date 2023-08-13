// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_TickWallRun.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWallRunSideDetermenedDelegate, bool, bLeftSide);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWallRunFinishedDelegate);

class ACharacter;
class UCharacterMovementComponent;

UCLASS()
class PARKOURANDMAGIC_API UAbilityTask_TickWallRun : public UAbilityTask
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FOnWallRunFinishedDelegate OnFinished;

    UPROPERTY(BlueprintAssignable)
    FOnWallRunSideDetermenedDelegate OnWallSideDetermened;

    UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HiddenPin = "OwningAbility", DefaultToSelf = "OwningAbility"))
    static UAbilityTask_TickWallRun* CreateWallRunTask(UGameplayAbility* OwningAbility, ACharacter* InCharacterOwner,
        UCharacterMovementComponent* InCharacterMovement, TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectType);

    virtual void Activate() override;
    virtual void OnDestroy(bool bInOwnerFinished) override;
    virtual void TickTask(float DeltaTime) override;

protected:
    UCharacterMovementComponent* CharacterMovement = nullptr;

    ACharacter* CharacterOwner = nullptr;

    TArray<TEnumAsByte<EObjectTypeQuery>> WallRun_TraceObjectTypes;

    bool FindRunnableWall(FHitResult& OnWallHit);
    bool isWallOnTheLeft(const FHitResult& InWallHit) const;
};
