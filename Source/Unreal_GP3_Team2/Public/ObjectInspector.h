// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ObjectInspector.generated.h"

class AItemBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREAL_GP3_TEAM2_API UObjectInspector : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UObjectInspector();

	UPROPERTY(Editanywhere, BlueprintReadWrite)
	FTransform ObjectOriginalPosition;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool CameFromEnvironment = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FVector ViewingPosition;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<AItemBase> ViewedItem = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool IsViewing = false;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	FInputModeUIOnly _objectInspectorInputMode = FInputModeUIOnly();


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void StartItemViewer(AItemBase* itemToView, bool fromEnvironment);

	UFUNCTION(BlueprintCallable)
	void CloseItemViewer();

	FVector GetViewingLocation();

};
