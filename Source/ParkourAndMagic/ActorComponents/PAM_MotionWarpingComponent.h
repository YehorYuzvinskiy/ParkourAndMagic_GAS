// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MotionWarpingComponent.h"
#include "PAM_GameTypes.h"
#include "PAM_MotionWarpingComponent.generated.h"

UCLASS()
class PARKOURANDMAGIC_API UPAM_MotionWarpingComponent : public UMotionWarpingComponent
{
    GENERATED_BODY()

public:
    UPAM_MotionWarpingComponent(const FObjectInitializer& ObjectInitazier);
    void SendWarpPointToClients();

    UFUNCTION(NetMulticast, Reliable)
    void MulticastSyncWarpPoint(const TArray<FMotionWarpingTargetByLocationAndRotation>& Targets);
};
