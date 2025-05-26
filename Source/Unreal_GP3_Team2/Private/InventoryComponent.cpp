// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "ItemBase.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	GotMirror = false;
	// ...
}

TArray<AItemBase*>& UInventoryComponent::GetActiveInventory()
{
	return IsCurrentTime ? CurrentInventoryItems : PastInventoryItems;
}

AItemBase*& UInventoryComponent::GetActiveItemAtHand()
{
	return IsCurrentTime ? CurrentItemAtHand : PastItemAtHand;
}

void UInventoryComponent::SwapTimeInInventory() 
{
	IsCurrentTime = !IsCurrentTime;
	OnInventoryUpdated.Broadcast();
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	IsCurrentTime = true;

	Test();
	
}

void UInventoryComponent::Test()
{
	/*UWorld* World = GetWorld();
	if (World)
	{
		AItemBase* NewItem = World->SpawnActor<AItemBase>(AItemBase::StaticClass());
		if (NewItem)
		{
			NewItem->ItemName = "TestKey";
			NewItem->ItemID = 1;
			NewItem->Description = FText::FromString("A key used for testing.");
			UTexture2D* LoadedTexture = LoadObject<UTexture2D>(nullptr, TEXT("Texture2D'/Game/Assets/Test/GenericKeyTesting.GenericKeyTesting'"));
			if (LoadedTexture)
			{
				NewItem->ItemIcon = LoadedTexture;
			}
			AddItem(NewItem);
		}
		AItemBase* NewSecondItem = World->SpawnActor<AItemBase>(AItemBase::StaticClass());
		if (NewSecondItem)
		{
			NewSecondItem->ItemName = "SecondTestKey";
			NewSecondItem->ItemID = 2;
			NewSecondItem->Description = FText::FromString("This is second test key.");
			UTexture2D* LoadedTexture = LoadObject<UTexture2D>(nullptr, TEXT("Texture2D'/Game/Assets/Test/GenericKeyTesting.GenericKeyTesting'"));
			if (LoadedTexture)
			{
				NewSecondItem->ItemIcon = LoadedTexture;
			}
			AddItem(NewSecondItem);
		}
		AItemBase* NewItemAtHand = World->SpawnActor<AItemBase>(AItemBase::StaticClass());
		if (NewItemAtHand)
		{
			NewItemAtHand->ItemName = "ItemAtHand";
			NewItemAtHand->ItemID = 0;
			NewItemAtHand->Description = FText::FromString("This is item at hand.");
			UTexture2D* LoadedTexture = LoadObject<UTexture2D>(nullptr, TEXT("Texture2D'/Game/Assets/Test/GenericWatchTesting.GenericWatchTesting'"));
			if (LoadedTexture)
			{
				NewItemAtHand->ItemIcon = LoadedTexture;
			}
			CurrentItemAtHand = NewItemAtHand;
			OnInventoryUpdated.Broadcast();
		}
	}*/
}

void UInventoryComponent::SwitchItem(AItemBase* TargetItem)
{
	if (!TargetItem)
	{
		UE_LOG(LogTemp, Warning, TEXT("Target item is null!"));
		return;
	}

	TArray<AItemBase*>& InventoryItems = GetActiveInventory();

	int32 Index = InventoryItems.IndexOfByKey(TargetItem);
	if (Index == INDEX_NONE)
	{
		UE_LOG(LogTemp, Warning, TEXT("Target item not found in inventory!"));
		return;
	}

	AItemBase*& ItemAtHand = GetActiveItemAtHand();

	if (!ItemAtHand)
	{
		ItemAtHand = TargetItem;
		InventoryItems.RemoveAt(Index);
	}
	else
	{
		InventoryItems[Index] = ItemAtHand;
		ItemAtHand = TargetItem;
	}
	OnInventoryUpdated.Broadcast();
}

void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInventoryComponent::AddItem(AItemBase* NewItem)
{
	if (!NewItem) return;

	TArray<AItemBase*>& InventoryItems = GetActiveInventory();
	
	InventoryItems.Add(NewItem);
	if (NewItem->ItemID == 23)	GotMirror = true;
	OnInventoryUpdated.Broadcast();
}

void UInventoryComponent::RemoveItem(AItemBase* OldItem)
{
	AItemBase*& ItemAtHand = GetActiveItemAtHand();

	if (OldItem == ItemAtHand) 
	{
		ItemAtHand = nullptr;
	} 
	else 
	{
		TArray<AItemBase*>& InventoryItems = GetActiveInventory();
		InventoryItems.Remove(OldItem);
	}
	OnInventoryUpdated.Broadcast();
}

