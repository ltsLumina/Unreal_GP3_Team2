// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectInspector.h"
#include "ItemBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"

// Sets default values for this component's properties
UObjectInspector::UObjectInspector()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UObjectInspector::BeginPlay()
{
	Super::BeginPlay();

	
	
}


// Called every frame
void UObjectInspector::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UObjectInspector::StartItemViewer(AItemBase* itemToView, bool fromEnvironment)
{
	IsViewing = true;

	GetOwner()->FindComponentByClass<UCharacterMovementComponent>()->DisableMovement();
	GetWorld()->GetFirstPlayerController()->SetIgnoreLookInput(true);
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
	GetWorld()->GetFirstPlayerController()->bEnableClickEvents = true;
	GetWorld()->GetFirstPlayerController()->bEnableMouseOverEvents = true;

	if (fromEnvironment)
	{
		ObjectOriginalPosition = itemToView->GetActorTransform();
		CameFromEnvironment = fromEnvironment;
	}

	ViewedItem = itemToView;
	itemToView->SetActorLocation(GetViewingLocation());

	//Disable Character Controls
	//Enable View Screen Controls

	//Move item in front of player
}

void UObjectInspector::CloseItemViewer()
{
	IsViewing = false;

	GetOwner()->FindComponentByClass<UCharacterMovementComponent>()->MovementMode = EMovementMode::MOVE_Walking;
	GetWorld()->GetFirstPlayerController()->SetIgnoreLookInput(false);
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;
	GetWorld()->GetFirstPlayerController()->bEnableClickEvents = false;
	GetWorld()->GetFirstPlayerController()->bEnableMouseOverEvents = false;


	if (CameFromEnvironment)
	{
		ViewedItem->SetActorTransform(ObjectOriginalPosition);
		CameFromEnvironment = false;
	}

	ViewedItem = nullptr;
	//Disable View Screen Controls
	//Enable Character Controls 

	//If from environment return to place
}

FVector UObjectInspector::GetViewingLocation()
{
	UStaticMeshComponent* viewPosObject = GetOwner()->FindComponentByTag<UStaticMeshComponent>(FName("ViewingPosition"));
	FVector viewingPosition = (viewPosObject->GetComponentLocation());

	return viewingPosition;
}

