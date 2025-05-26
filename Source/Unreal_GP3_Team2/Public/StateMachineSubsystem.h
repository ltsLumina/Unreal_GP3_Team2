// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "StateMachineSubsystem.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EGlobalState : uint8
{
	Init,
	Menu,
	Intro,
	Play,
	Pause,
	Outro,
	Quit,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGlobalStateChanged, EGlobalState, OldGlobalState, EGlobalState, NewGlobalState);

UCLASS()
class UNREAL_GP3_TEAM2_API UStateMachineSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
private:
	UPROPERTY()
	EGlobalState GlobalState = EGlobalState::Init;

public:
	UPROPERTY(BlueprintAssignable, Category = "GlobalState")
	FOnGlobalStateChanged OnGlobalStateChanged;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE EGlobalState GetGameState() const { return GlobalState; }

	UFUNCTION(BlueprintCallable)
	void ChangeState(EGlobalState InNewState);

};
