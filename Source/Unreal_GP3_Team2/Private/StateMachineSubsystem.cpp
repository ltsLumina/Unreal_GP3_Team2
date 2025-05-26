// Fill out your copyright notice in the Description page of Project Settings.


#include "StateMachineSubsystem.h"


void UStateMachineSubsystem::ChangeState(EGlobalState InNewState)
{
	if (GlobalState == InNewState)
	{
		return;
	}

	EGlobalState OldState = GlobalState;

	GlobalState = InNewState;

	OnGlobalStateChanged.Broadcast(OldState, GlobalState);
}