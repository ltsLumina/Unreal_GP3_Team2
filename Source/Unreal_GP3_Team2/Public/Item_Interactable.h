// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "Item_Interactable.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_GP3_TEAM2_API AItem_Interactable : public AItemBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Details")
	TArray<FInteractionOption> InteractionActions;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE TArray<FInteractionOption> GetUserActions() { return InteractionActions; };

	UFUNCTION(BlueprintCallable)
	void Interact();


	UFUNCTION(BlueprintNativeEvent)
	void OnInteract();

	void OnInteract_Implementation();


};
