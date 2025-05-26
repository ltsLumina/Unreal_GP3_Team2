// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item_Interactable.h"
#include "Item_Book.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_GP3_TEAM2_API AItem_Book : public AItem_Interactable
{
	GENERATED_BODY()

public: 

	UFUNCTION(BlueprintCallable)
	void Read();


	UFUNCTION(BlueprintNativeEvent)
	void OnRead();

	void OnRead_Implementation();
};
