// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PAM_GameInstance.generated.h"

/**
 *
 */
UCLASS()
class PARKOURANDMAGIC_API UPAM_GameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    virtual void Init() override;
};
