// Fill out your copyright notice in the Description page of Project Settings.


#include "CombinationSafe.h"

void ACombinationSafe::BeginPlay()
{
	Super::BeginPlay();

	ArcPerDial = 360 / NumberDialSides;


}
