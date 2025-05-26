// Copyright Epic Games, Inc. All Rights Reserved.

#include "Unreal_GP3_Team2PickUpComponent.h"

UUnreal_GP3_Team2PickUpComponent::UUnreal_GP3_Team2PickUpComponent()
{
	// Setup the Sphere Collision
	SphereRadius = 32.f;
}

void UUnreal_GP3_Team2PickUpComponent::BeginPlay()
{
	Super::BeginPlay();

	// Register our Overlap Event
	OnComponentBeginOverlap.AddDynamic(this, &UUnreal_GP3_Team2PickUpComponent::OnSphereBeginOverlap);
}

void UUnreal_GP3_Team2PickUpComponent::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Checking if it is a First Person Character overlapping
	AUnreal_GP3_Team2Character* Character = Cast<AUnreal_GP3_Team2Character>(OtherActor);
	if(Character != nullptr)
	{
		// Notify that the actor is being picked up
		OnPickUp.Broadcast(Character);

		// Unregister from the Overlap Event so it is no longer triggered
		OnComponentBeginOverlap.RemoveAll(this);
	}
}
