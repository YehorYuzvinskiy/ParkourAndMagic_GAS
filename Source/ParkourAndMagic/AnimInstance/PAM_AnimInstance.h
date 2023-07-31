// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PAM_AnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PARKOURANDMAGIC_API UPAM_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
    UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
    class UBlendSpace* GetLocomotionBlendSpace() const;

    UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
    class UAnimSequenceBase* GetIdleAnimation() const;

        UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
    class UBlendSpace* GetCrouchBlendSpace() const;

    UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
    class UAnimSequenceBase* GetCrouchIdleAnimation() const;

      UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Animation")
    class UCharacterAnimationDataAsset* DefaultCharacterAnimDataAsset;
};
