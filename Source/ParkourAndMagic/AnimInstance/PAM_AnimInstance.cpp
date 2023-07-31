// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance/PAM_AnimInstance.h"
#include "PAM_GameTypes.h"
#include "Animation/AnimSequenceBase.h"
#include "Animation/BlendSpace.h"
#include "DataAssets/CharacterDataAsset.h"
#include "DataAssets/CharacterAnimationDataAsset.h"
#include "ParkourAndMagicCharacter.h"

UBlendSpace* UPAM_AnimInstance::GetLocomotionBlendSpace() const
{
    if(AParkourAndMagicCharacter* ParkourAndMagicCharacter = Cast<AParkourAndMagicCharacter> (GetOwningActor()))
    {
        FCharacterData Data = ParkourAndMagicCharacter->GetCharacterData();
        if(Data.CharacterAnimDataAsset)
        {
            return Data.CharacterAnimDataAsset->CharacterAnimationData.MovementBlendSpace;
        }
    }
    return DefaultCharacterAnimDataAsset ? DefaultCharacterAnimDataAsset->CharacterAnimationData.MovementBlendSpace : nullptr;
}

UAnimSequenceBase* UPAM_AnimInstance::GetIdleAnimation() const
{
    if (AParkourAndMagicCharacter* ParkourAndMagicCharacter = Cast<AParkourAndMagicCharacter>(GetOwningActor()))
    {
        FCharacterData Data = ParkourAndMagicCharacter->GetCharacterData();
        if (Data.CharacterAnimDataAsset)
        {
            return Data.CharacterAnimDataAsset->CharacterAnimationData.IdleAnimationAsset;;
        }
    }
    return DefaultCharacterAnimDataAsset ? DefaultCharacterAnimDataAsset->CharacterAnimationData.IdleAnimationAsset : nullptr;
}

UBlendSpace* UPAM_AnimInstance::GetCrouchBlendSpace() const
{
    if (AParkourAndMagicCharacter* ParkourAndMagicCharacter = Cast<AParkourAndMagicCharacter>(GetOwningActor()))
    {
        FCharacterData Data = ParkourAndMagicCharacter->GetCharacterData();
        if (Data.CharacterAnimDataAsset)
        {
            return Data.CharacterAnimDataAsset->CharacterAnimationData.CrouchBlendSpace;
        }
    }
    return DefaultCharacterAnimDataAsset ? DefaultCharacterAnimDataAsset->CharacterAnimationData.CrouchBlendSpace : nullptr;
}

UAnimSequenceBase* UPAM_AnimInstance::GetCrouchIdleAnimation() const
{
    if (AParkourAndMagicCharacter* ParkourAndMagicCharacter = Cast<AParkourAndMagicCharacter>(GetOwningActor()))
    {
        FCharacterData Data = ParkourAndMagicCharacter->GetCharacterData();
        if (Data.CharacterAnimDataAsset)
        {
            return Data.CharacterAnimDataAsset->CharacterAnimationData.CrouchIdleAnimationAsset;
            ;
        }
    }
    return DefaultCharacterAnimDataAsset ? DefaultCharacterAnimDataAsset->CharacterAnimationData.CrouchIdleAnimationAsset : nullptr;
}
