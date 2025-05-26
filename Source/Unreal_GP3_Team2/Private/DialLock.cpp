// Fill out your copyright notice in the Description page of Project Settings.


#include "DialLock.h"

void ADialLock::BeginPlay()
{
	Super::BeginPlay();

	DialFace = FindComponentByTag<UStaticMeshComponent>(FName("DialFace"));
	InputArc = 360.0f / MaxInput;

	for (int i = 0; i < CodeSequence.Num(); i++)
	{
		if (CodeSequence[i] >= MaxInput)
		{
			CodeSequence[i] = CodeSequence[i] % MaxInput;
		}
	}
}

bool ADialLock::CheckDirChange()
{

	return CurrentSpinDirection == PrevSpinDirection;
}

void ADialLock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	GetCurrentDialValue();

}

void ADialLock::ResetAttempt()
{
	LastValidIndex = -1;
	_rotation = 0;
	FRotator rotation = FRotator(0, 0, 90);
	DialFace->SetRelativeRotation(rotation);
}

float ADialLock::GetCurrentDialValue()
{
	float value = FMath::Fmod(_rotation, 360.0f) / InputArc;
	return value;
}

void ADialLock::SetSpinDirection(ESpinDirection inDir)
{
	PrevSpinDirection = CurrentSpinDirection;
	CurrentSpinDirection = inDir;
}

void ADialLock::RotateDial(float rotatedAmount)
{
	_rotation += rotatedAmount;
	if (_rotation < 0) _rotation += 360;
	FRotator rotation = FRotator(_rotation, 0, 90);
	DialFace->SetRelativeRotation(rotation);
}
