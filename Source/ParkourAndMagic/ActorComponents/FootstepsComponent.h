// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PAM_GameTypes.h"
#include "FootstepsComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PARKOURANDMAGIC_API UFootstepsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFootstepsComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
    FName LeftFootSocketName = TEXT("Foot_l");

	UPROPERTY(EditDefaultsOnly)
    FName RightFootSocketName = TEXT("foot_r");

public:	
	
	void HandleFootStep(EFoot Foot);
		
};
