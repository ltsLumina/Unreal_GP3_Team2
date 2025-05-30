// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item_Interactable.h"
#include "GrandpopClock_Item_Interactable.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_GP3_TEAM2_API AGrandpopClock_Item_Interactable : public AItem_Interactable
{
    GENERATED_BODY()

public:
    AGrandpopClock_Item_Interactable();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void PostInitializeComponents() override;

protected:
    void UpdatePendulum(float DeltaTime);
    void SwitchPendulumDirection();
    void TranslateTimeToRotation(float& OutHourRotation, float& OutMinuteRotation);
    void SetTargetRotation(float DeltaTime, float TargetHourRotation, float TargetMinuteRotation);
    void SetTimeInRotation(int32 Hour, int32 Minute, float& OutHourRotation, float& OutMinuteRotation);
    UFUNCTION(BlueprintCallable)
    void RepairClock();

    UPROPERTY(VisibleAnywhere)
    USceneComponent* Root;

    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* MainBody;

    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* BigPointer;

    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* SmallPointer;

    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* Pendulum;

    UPROPERTY(EditAnywhere, Category = "Clock")
    float SwingPeriod = 2.0f;

    UPROPERTY(EditAnywhere, Category = "Clock")
    float MaxSwingAngle = 4.0f;

    UPROPERTY(EditAnywhere, Category = "Clock")
    float RotationInterpSpeed = 0.1f;

    UPROPERTY()
    bool PresentWorld;

    UPROPERTY()
    bool bIsSwingingForward = true;

    UPROPERTY()
    float ElapsedTime = 0.0f;

    UPROPERTY()
    FTimerHandle PendulumTimerHandle;

    UPROPERTY()
    float FirstTime;

    UPROPERTY()
    float SecondTime;

    UPROPERTY()
    bool bShouldRotate;

};