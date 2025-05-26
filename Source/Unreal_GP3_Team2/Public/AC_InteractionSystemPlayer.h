// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/PlayerController.h"
#include "ItemBase.h"
#include "AC_InteractionSystemPlayer.generated.h"

class AItemBase;
class UObjectInspector;
class UInventoryComponent;


USTRUCT(BlueprintType)
struct FTracePoints
{
	GENERATED_BODY();
public:
	FVector StartPos = FVector().ZeroVector;
	FVector EndPos = FVector().ZeroVector;

	bool Validate()
	{
		if (StartPos == EndPos)
			return false;
		else
			return true;
	}
};



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREAL_GP3_TEAM2_API UAC_InteractionSystemPlayer : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAC_InteractionSystemPlayer();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InteractionDistance = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DebugInfo")
	bool ShowLineTraces = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<AItemBase> CurrentHighlighted = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<AItemBase> PrevHighlighted = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool SomethingLookedAt = false;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<APlayerController> _firstPlayerController;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UObjectInspector> _objectInspectorComponent;

	UPROPERTY()
	APlayerController* playerController;

	UPROPERTY()
	UInventoryComponent* Inventory;

	UFUNCTION()
	void TryToInit();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	FTracePoints GetTraces();

	UFUNCTION(BlueprintCallable)
	bool InteractionTrace(FHitResult& result);

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool GetSomethingLookedAt() { return SomethingLookedAt; }

	UFUNCTION(BlueprintCallable)
	void ViewItem(AItemBase* itemToView);

	UFUNCTION(BlueprintCallable)
	void StashItem(AItemBase* itemToStash);
};
