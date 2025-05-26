// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryUIComponent.h"
#include "InventoryComponent.h"

// Sets default values for this component's properties
UInventoryUIComponent::UInventoryUIComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInventoryUIComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	if (UInventoryComponent* Inv = GetOwner()->FindComponentByClass<UInventoryComponent>())
	{
		Inv->OnInventoryUpdated.AddDynamic(this, &UInventoryUIComponent::HandleInventoryUpdated);
	}
	
}

void UInventoryUIComponent::HandleInventoryUpdated()
{
	OnInventoryChanged();
}


// Called every frame
void UInventoryUIComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

