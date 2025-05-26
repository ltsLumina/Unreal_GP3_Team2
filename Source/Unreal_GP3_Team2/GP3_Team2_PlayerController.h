// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GP3_Team2_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_GP3_TEAM2_API AGP3_Team2_PlayerController : public APlayerController
{
    GENERATED_BODY()

protected:
    virtual void BeginPlay() override;

    AGP3_Team2_PlayerController();

    ULevelStreaming* LoadStreamingLevel(UObject* WorldContextObject, const FName& LevelName, bool bActive);

    UFUNCTION()
    void PresentLoaded();

    UFUNCTION()
    void PastLoaded();

    UFUNCTION()
    void PresentLevelShown();

    UFUNCTION()
    void PastLevelShown();

    void ChangeActiveWorld(ULevelStreaming* InActivate, ULevelStreaming* Activate);

public:
    UFUNCTION(BlueprintCallable, Category = "Level")
    void SwapActiveLevel();

private:
    UPROPERTY(Transient)
    ULevelStreaming* Present;

    UPROPERTY(Transient)
    ULevelStreaming* Past;

    UPROPERTY(Transient)
    ULevelStreaming* ActiveLevel;
};