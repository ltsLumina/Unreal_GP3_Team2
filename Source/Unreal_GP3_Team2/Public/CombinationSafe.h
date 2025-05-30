// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item_Interactable.h"
#include "CombinationSafe.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_GP3_TEAM2_API ACombinationSafe : public AItem_Interactable
{
	GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dial Information")
    UStaticMeshComponent* Dial1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dial Information")
    UStaticMeshComponent* Dial2;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dial Information")
    UStaticMeshComponent* Dial3;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dial Information")
    UStaticMeshComponent* Dial4;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Dial Information")
    int NumberOfDials = 4;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dial Information")
    int NumberDialSides = 7;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Dial Information")
    float ArcPerDial = 360 / NumberDialSides;

protected:
    virtual void BeginPlay() override;
};

