// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GP3_Team2_GameState.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_GP3_TEAM2_API AGP3_Team2_GameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, Category = "Doors")
	TMap<int32, bool> DoorStates;
};
