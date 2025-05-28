// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "Item_Pickup.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_GP3_TEAM2_API AItem_Pickup : public AItemBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Details")
	TArray<FInteractionOption> InspectionActions;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE TArray<FInteractionOption> GetUserActions() { return InspectionActions; };
	

};
