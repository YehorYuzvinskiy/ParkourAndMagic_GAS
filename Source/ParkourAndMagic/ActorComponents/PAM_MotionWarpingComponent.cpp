// Fill out your copyright notice in the Description page of Project Settings.

#include "ActorComponents/PAM_MotionWarpingComponent.h"

UPAM_MotionWarpingComponent::UPAM_MotionWarpingComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    SetIsReplicatedByDefault(true);
}

/*
void UPAM_MotionWarpingComponent::SendWarpPointToClients() 
{
    if (GetOwnerRole() == ROLE_Authority)
    {
        TArray<FMotionWarpingTargetByLocationAndRotation> WarpTargets;

        
        for (auto WarpTarget : WarpTargetMap)
        {
            FMotionWarpingTargetByLocationAndRotation MotionWarpingTarget(
                WarpTarget.Key, WarpTarget.Value.GetLocation(), WarpTarget.Value.GetRotation());

            WarpTargets.Add(MotionWarpingTarget);
        }
        MulticastSyncWarpPoint(WarpTargets);
    }
}

void UPAM_MotionWarpingComponent::MulticastSyncWarpPoint_Implementation(const TArray<FMotionWarpingTargetByLocationAndRotation>& Targets) 
{
    for (const FMotionWarpingTargetByLocationAndRotation& Target : Targets)
    {
        AddOrUpdateWarpTargetFromLocationAndRotation(Target.Name, Target.Location, FRotator(Target.Rotation));
    }
}
*/
