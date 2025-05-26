// Fill out your copyright notice in the Description page of Project Settings.


#include "Item_Interactable.h"

TArray<EInteractionOptions> AItem_Interactable::GetInteractOptions()
{
	TArray<EInteractionOptions> options = TArray<EInteractionOptions>();
	options.Add(EInteractionOptions::INTERACT);

	return options;
}

void AItem_Interactable::Interact()
{
	OnInteract();
}

void AItem_Interactable::OnInteract_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Native C++ OnInteract implementation"));
}
