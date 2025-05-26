// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class AItemBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREAL_GP3_TEAM2_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryUpdated OnInventoryUpdated;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	bool IsCurrentTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	bool GotMirror;

	// Sets default values for this component's properties
	UInventoryComponent();

	UFUNCTION(BlueprintCallable)
	void AddItem(AItemBase* NewItem);

	UFUNCTION(BlueprintCallable)
	void RemoveItem(AItemBase* NewItem);

	UFUNCTION(BlueprintCallable)
	void SwitchItem(AItemBase* TargetItem);

	UFUNCTION(BlueprintPure)
	TArray<AItemBase*>& GetActiveInventory();

	UFUNCTION(BlueprintPure)
	AItemBase*& GetActiveItemAtHand();

	UFUNCTION(BlueprintCallable)
	void SwapTimeInInventory();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void Test();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintReadOnly)
	TArray<AItemBase*> CurrentInventoryItems;

	UPROPERTY(BlueprintReadOnly)
	AItemBase* CurrentItemAtHand;

	UPROPERTY(BlueprintReadOnly)
	TArray<AItemBase*> PastInventoryItems;

	UPROPERTY(BlueprintReadOnly)
	AItemBase* PastItemAtHand;

		
};
