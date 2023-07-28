// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimNotifys/AnimNotify_Footsteps.h"

#include "ParkourAndMagicCharacter.h"
#include "ActorComponents/FootstepsComponent.h"

void UAnimNotify_Footsteps::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    Super::Notify(MeshComp, Animation);

    check(MeshComp);

    AParkourAndMagicCharacter* Character = MeshComp ? Cast<AParkourAndMagicCharacter>(MeshComp->GetOwner()) : nullptr;
    if (Character)
    {
        if (UFootstepsComponent* FootstepsComponent = Character->GetFootstepsComponent())
        {
            FootstepsComponent->HandleFootStep(Foot);
        }
    }
}
