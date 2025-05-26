// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBase.h"
#include "GameFramework/PlayerController.h"



// Sets default values
AItemBase::AItemBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AItemBase::BeginPlay()
{
	Super::BeginPlay();
}

void AItemBase::LookEnter()
{
	_hasEntered = true;
	_hasLeft = false;
	OnLookEnter();
}

void AItemBase::LookExit()
{
	_hasEntered = false;
	_hasLeft = true;
	OnLookExit();
}

// Called every frame
void AItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItemBase::OnLookEnter_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Native C++ OnLookEnter implementation"));
}

void AItemBase::OnLookExit_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Native C++ OnLookExit implementation"));

}

TArray<EInteractionOptions> AItemBase::GetInteractOptions()
{
	TArray<EInteractionOptions> options = TArray<EInteractionOptions>();
	options.Add(EInteractionOptions::NONE);

	return options;
}

void AItemBase::Look()
{
	if (!_hasEntered)
	{
		LookEnter();
		//UE_LOG(LogTemp, Warning, TEXT("Item Look entered At!"));
	}
	//UE_LOG(LogTemp, Warning, TEXT("Item Looked At!"));
}

void AItemBase::LookAway()
{
	if (_hasLeft)
		return;
	LookExit();
	//UE_LOG(LogTemp, Warning, TEXT("Item Looked Away!"));

}

