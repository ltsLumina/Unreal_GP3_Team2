// Fill out your copyright notice in the Description page of Project Settings.


#include "Item_Pickup.h"

TArray<EInteractionOptions> AItem_Pickup::GetInteractOptions()
{
	TArray<EInteractionOptions> options = TArray<EInteractionOptions>();
	options.Add(EInteractionOptions::PICKUP);
	options.Add(EInteractionOptions::STASH);

	return options;
}
