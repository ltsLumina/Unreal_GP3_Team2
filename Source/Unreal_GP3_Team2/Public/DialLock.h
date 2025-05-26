// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item_Interactable.h"
#include "DialLock.generated.h"

UENUM(BlueprintType)
enum class ESpinDirection : uint8
{
	NONE = 0, LEFT = 1, RIGHT = 2
};


/**
 * For those that don't know how a combination lock works. (I had forgotton)
 * In general to open a combination lock you turn the dial in an alternating 
 * direction to each number, usually starting with counter-clockwise (left).
 * There is usually some mechnism to reset the sequence so you can start again
 * if you mess up. In the game this will just be a button.
 */
UCLASS()
class UNREAL_GP3_TEAM2_API ADialLock : public AItem_Interactable
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LockDetails")
	TObjectPtr<UStaticMeshComponent> DialFace;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LockDetails")
	int MaxInput = 60;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "LockDetails")
	float InputArc = 6;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LockDetails")
	int LockInputCount = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LockDetails")
	TArray<int> CodeSequence;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "LockDetails")
	int LastValidIndex = -1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "LockDetails")
	ESpinDirection PrevSpinDirection = ESpinDirection::NONE;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "LockDetails")
	ESpinDirection CurrentSpinDirection = ESpinDirection::NONE;

protected:
	virtual void BeginPlay() override;

private:
	float _rotation = 0;

	bool CheckDirChange();

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void ResetAttempt();

	UFUNCTION(BlueprintCallable)
	float GetCurrentDialValue();

	UFUNCTION(BlueprintCallable)
	void SetSpinDirection(ESpinDirection inDir);

	UFUNCTION(BlueprintCallable)
	void RotateDial(float rotatedAmount);
	
};
