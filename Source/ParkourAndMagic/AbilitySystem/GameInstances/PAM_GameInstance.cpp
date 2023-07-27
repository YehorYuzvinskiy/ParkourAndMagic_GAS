// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/GameInstances/PAM_GameInstance.h"
#include <AbilitySystemGlobals.h>

void UPAM_GameInstance::Init()
{
    Super::Init();

    UAbilitySystemGlobals::Get().InitGlobalData();
}