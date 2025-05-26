#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Item_Interactable.h"
#include "Delegates/Delegate.h"
#include "CombinationLock.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCombinationLockUnlocked);

USTRUCT()
struct FDialRotationData
{
    GENERATED_BODY()

    UStaticMeshComponent* Dial = nullptr;
    int32 CurrentValue = 0;
    int32 TargetValue = 0;
    bool bIsRotating = false;
};

UCLASS()
class UNREAL_GP3_TEAM2_API ACombinationLock : public AItem_Interactable
{
	GENERATED_BODY()

public:
	ACombinationLock();

protected:
	virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dials")
    UStaticMeshComponent* Dial1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dials")
    UStaticMeshComponent* Dial2;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dials")
    UStaticMeshComponent* Dial3;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dials")
    UStaticMeshComponent* Dial4;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dials")
    UStaticMeshComponent* Dial5;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dials")
    UStaticMeshComponent* SelectedDial;

    UPROPERTY(BlueprintReadWrite, Category = "Values")
    TMap<UStaticMeshComponent*, int32> TargetValues;

private:
    UPROPERTY()
    int32 SwitchValue;

    UPROPERTY()
    TArray<bool> TargetsFound;

    UPROPERTY()
    TArray<FDialRotationData> DialsToAnimate;

    UFUNCTION()
    void InitialSetting();

    UFUNCTION()
    int32 GetDialValue(UStaticMeshComponent* Dial);

public:
    UFUNCTION(BlueprintCallable, Category = "LockFunctions", meta = (DisplayName = "Rotate Dial"))
    void OnDialClicked(UStaticMeshComponent* Dial, int32 Direction);

    UFUNCTION(BlueprintCallable, Category = "LockFunctions")
    bool CheckValues();

    UFUNCTION(BlueprintCallable, Category = "LockFunctions", meta = (DisplayName = "Set Selected Dial"))
    void SwitchSelectedDial(int32 value);

    UFUNCTION(BlueprintPure, Category = "LockFunctions", meta = (DisplayName = "Is Any Dial Rotating"))
    bool IsAnyDialAnimating(); 
    
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FCombinationLockUnlocked OnSomethingHappened;

    void TriggerEvent();
    

};