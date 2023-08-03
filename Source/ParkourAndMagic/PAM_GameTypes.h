// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PAM_GameTypes.generated.h"

USTRUCT(BlueprintType)
struct FCharacterData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GAS")
    TArray<TSubclassOf<class UGameplayEffect>> Effects;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GAS")
    TArray<TSubclassOf<class UGameplayAbility>> Abilities;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Animation")
    class UCharacterAnimationDataAsset* CharacterAnimDataAsset;
};

USTRUCT(BlueprintType)
struct FCharacterAnimationData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly)
    class UBlendSpace* MovementBlendSpace = nullptr;

    UPROPERTY(EditDefaultsOnly)
    class UAnimSequenceBase* IdleAnimationAsset = nullptr;

       UPROPERTY(EditDefaultsOnly)
    class UBlendSpace* CrouchBlendSpace = nullptr;

    UPROPERTY(EditDefaultsOnly)
    class UAnimSequenceBase* CrouchIdleAnimationAsset = nullptr;
};

UENUM(BlueprintType)
enum class EFoot : uint8
{
    Left UMETA(DisaplayName = "Left"),
    Right UMETA(DisplayName = "Right")
};

USTRUCT(BlueprintType)
struct FMotionWarpingTargetByLocationAndRotation
{
    GENERATED_USTRUCT_BODY();

    FMotionWarpingTargetByLocationAndRotation() {}

    FMotionWarpingTargetByLocationAndRotation(FName InName, FVector InLocation, FQuat InRotation)
        : Name(InName), Location(InLocation), Rotation(InRotation)
    {
    }
    UPROPERTY()
    FName Name;
    UPROPERTY()
    FVector Location;

    UPROPERTY()
    FQuat Rotation;
};