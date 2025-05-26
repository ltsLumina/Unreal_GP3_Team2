// Fill out your copyright notice in the Description page of Project Settings.


#include "AC_InteractionSystemPlayer.h"

#include "ObjectInspector.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Controller.h"
#include "InventoryComponent.h"

// Sets default values for this component's properties
UAC_InteractionSystemPlayer::UAC_InteractionSystemPlayer()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	playerController = nullptr;
	// ...
}


// Called when the game starts
void UAC_InteractionSystemPlayer::BeginPlay()
{
	Super::BeginPlay();

	_firstPlayerController = GetWorld()->GetFirstPlayerController();
	_objectInspectorComponent = GetOwner()->FindComponentByClass<UObjectInspector>();
	ACharacter* MyCharacter = Cast<ACharacter>(GetOwner());
	if (MyCharacter)
	{
		// Set a timer to check for possession (delay of 0.2 seconds)
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UAC_InteractionSystemPlayer::TryToInit, 0.2f, false);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("GetOwner() is not a Character."));
	}
}
void UAC_InteractionSystemPlayer::TryToInit()
{
	ACharacter* MyCharacter = Cast<ACharacter>(GetOwner());
	if (MyCharacter)
	{
		AController* Controller = MyCharacter->GetController();
		playerController = Cast<APlayerController>(Controller);

		if (playerController)
		{
			Inventory = playerController->FindComponentByClass<UInventoryComponent>();
			if (Inventory)
			{
				UE_LOG(LogTemp, Warning, TEXT("Inventory found on PlayerController."));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Inventory component not found on PlayerController."));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to get PlayerController (Controller is nullptr)."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get Character (GetOwner() is not valid)."));
	}
}

// Called every frame
void UAC_InteractionSystemPlayer::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FHitResult hit;
	if (InteractionTrace(hit))
	{
		AItemBase* itemHit = Cast<AItemBase>(hit.GetActor());
		if (IsValid(itemHit))
		{
			CurrentHighlighted = itemHit;
			if (IsValid(PrevHighlighted) && CurrentHighlighted != PrevHighlighted)
			{
				PrevHighlighted->LookAway();
			}
			itemHit->Look();
			PrevHighlighted = CurrentHighlighted;
		}
		SomethingLookedAt = true;
	}
	else
	{
		if (IsValid(CurrentHighlighted))
		{
			CurrentHighlighted->LookAway();
			CurrentHighlighted = nullptr;
			SomethingLookedAt = false;
		}
	}

	
}

FTracePoints UAC_InteractionSystemPlayer::GetTraces()
{
	int viewportX, viewportY;
	_firstPlayerController->GetViewportSize(viewportX, viewportY);

	auto ScreenLocation = FVector2D(viewportX/2, viewportY/2);

	FVector WorldLocation;
	FVector WorldDirection;
	FTracePoints points = FTracePoints();

	if (_firstPlayerController->DeprojectScreenPositionToWorld(ScreenLocation.X, ScreenLocation.Y, WorldLocation, WorldDirection))
	{
		FVector startPos = WorldLocation;
		FVector endPos = startPos + (WorldDirection * InteractionDistance);

		
		points.StartPos = startPos;
		points.EndPos = endPos;

		return points;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Deprojection Failed"));
	}
	return points;
}

bool UAC_InteractionSystemPlayer::InteractionTrace(FHitResult& result)
{
	FTracePoints positions = GetTraces();
	if (!positions.Validate())
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid TracePoints"));
		return false;
	}

	FHitResult blockingCheck;

	FCollisionQueryParams qp;
	qp.AddIgnoredActor(GetOwner());
	FCollisionResponseParams resp;
	GetWorld()->LineTraceSingleByChannel(result, positions.StartPos, positions.EndPos, ECollisionChannel::ECC_GameTraceChannel2, qp, resp);
	GetWorld()->LineTraceSingleByChannel(blockingCheck, positions.StartPos, positions.EndPos, ECollisionChannel::ECC_WorldStatic, qp, resp);
	
	if (blockingCheck.bBlockingHit && blockingCheck.Distance < result.Distance)
	{
		if (ShowLineTraces)
			DrawDebugLine(GetWorld(), blockingCheck.TraceStart, blockingCheck.TraceEnd, FColor::Red, false, 1, 0, 3);
		return false;
	}

	
	if (result.bBlockingHit && ShowLineTraces)
		DrawDebugLine(GetWorld(), positions.StartPos, positions.EndPos, FColor::Emerald, false, 1, 0, 3);
	else if (ShowLineTraces)
		DrawDebugLine(GetWorld(), positions.StartPos, positions.EndPos, FColor::Red, false, 1, 0, 3);
	
	if (result.bBlockingHit)
		return true;
	return false;

}

void UAC_InteractionSystemPlayer::ViewItem(AItemBase* itemToView)
{
	_objectInspectorComponent->StartItemViewer(itemToView, true);
}

void UAC_InteractionSystemPlayer::StashItem(AItemBase* itemToStash)
{
	if (Inventory)
	{
		Inventory->AddItem(itemToStash);
		UE_LOG(LogTemp, Warning, TEXT("Inventory: %s, has added: %s"),
			*Inventory->GetName(),
			*itemToStash->GetName());

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Inventory is Nullptr"));
	}
}