#include "CombinationLock.h"
#include "Math/UnrealMathUtility.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ACombinationLock::ACombinationLock()
{
    PrimaryActorTick.bCanEverTick = true;

    SelectedDial = nullptr;
    SwitchValue = 0;
}

// Called when the game starts or when spawned
void ACombinationLock::BeginPlay()
{
    Super::BeginPlay();

    InitialSetting();

    SelectedDial = Dial1;
}

// Called every frame
void ACombinationLock::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    constexpr float DegreesPerStep = 30.0f;
    constexpr float RotationSpeed = 70.0f; // degrees/second
    constexpr float Tolerance = 0.5f;

    for (int32 i = DialsToAnimate.Num() - 1; i >= 0; --i)
    {
        FDialRotationData& Entry = DialsToAnimate[i];
        if (!Entry.bIsRotating || !Entry.Dial) continue;

        float TargetPitch = Entry.TargetValue * DegreesPerStep;
        float CurrentPitch = Entry.Dial->GetRelativeRotation().Roll;

        float NewPitch = FMath::FixedTurn(CurrentPitch, TargetPitch, RotationSpeed * DeltaTime);

        FRotator NewRot = Entry.Dial->GetRelativeRotation();
        NewRot.Roll = NewPitch;
        Entry.Dial->SetRelativeRotation(NewRot);

        if (FMath::Abs(FMath::FindDeltaAngleDegrees(NewPitch, TargetPitch)) < Tolerance)
        {
            Entry.CurrentValue = Entry.TargetValue;
            Entry.bIsRotating = false;
            Entry.Dial->SetRelativeRotation(FRotator(NewRot.Pitch, NewRot.Yaw, TargetPitch));
            UE_LOG(LogTemp, Warning, TEXT("Dial %s reached value %d"), *Entry.Dial->GetName(), Entry.CurrentValue);
            DialsToAnimate.RemoveAt(i);
            CheckValues();
        }
    }
    CheckValues();
}

void ACombinationLock::InitialSetting()
{
    constexpr float DegreesPerStep = 30.0f;

    TArray<UStaticMeshComponent*> Dials = { Dial1, Dial2, Dial3, Dial4, Dial5 };

    for (UStaticMeshComponent* Dial : Dials)
    {
        int32 RandomValue = FMath::RandRange(0, 11);
        FRotator Rotator = Dial->GetRelativeRotation();
        Rotator.Roll = RandomValue * DegreesPerStep;
        Dial->SetRelativeRotation(Rotator);

        UE_LOG(LogTemp, Warning, TEXT("Dial Value is: %d"), RandomValue);
    }
}

bool ACombinationLock::CheckValues()
{
    TargetsFound.Empty();

    constexpr float DegreesPerStep = 30.0f;

    TArray<UStaticMeshComponent*> Dials = { Dial1, Dial2, Dial3, Dial4, Dial5 };

    for (UStaticMeshComponent* Dial : Dials)
    {
        if (TargetValues.Contains(Dial))
        {
            int32 ExpectedValue = TargetValues[Dial];
            float Pitch = Dial->GetRelativeRotation().Roll;
            float NormalizedPitch = FMath::Fmod(Pitch, 360.0f);
            if (FMath::IsNearlyZero(NormalizedPitch, 0.01f))
            {
                NormalizedPitch = 0.0f;
            }
            else if (NormalizedPitch < 0)
            {
                NormalizedPitch += 360.0f;
            }

            NormalizedPitch = FMath::Clamp(NormalizedPitch, 0.0f, 359.999f);

            int32 CurrentValue = FMath::RoundToInt(NormalizedPitch / DegreesPerStep) % 12;

            bool bIsCorrect = (ExpectedValue == CurrentValue);
            TargetsFound.Add(bIsCorrect);

            UE_LOG(LogTemp, Warning, TEXT("Dial %s - Expected: %d, Actual: %d, Pitch: %.1f"),
                *Dial->GetName(), ExpectedValue, CurrentValue, Pitch);
        }
        else
        {
            TargetsFound.Add(false);
            UE_LOG(LogTemp, Warning, TEXT("Missing dial detected, dial %s"), *Dial->GetName());
        }
    }

    for (bool bFound : TargetsFound)
    {
        if (!bFound)
        {
            UE_LOG(LogTemp, Warning, TEXT("Value found to be false"));
            TargetsFound.Empty();
            return false;
        }
        UE_LOG(LogTemp, Warning, TEXT("Value found to be true"));
    }

    UE_LOG(LogTemp, Warning, TEXT("All values correct."));
    TargetsFound.Empty();
    TriggerEvent();
    return true;
}

bool ACombinationLock::IsAnyDialAnimating() 
{
    return DialsToAnimate.Num() > 0;
}

void ACombinationLock::OnDialClicked(UStaticMeshComponent* Dial, int32 Direction)
{
    for (FDialRotationData& Entry : DialsToAnimate)
    {
        if (Entry.Dial == Dial)
        {
            Entry.TargetValue = (Entry.TargetValue + Direction) % 12;
            if (Entry.TargetValue < 0) Entry.TargetValue += 12;
            Entry.bIsRotating = true;
            return;
        }
    }

    FDialRotationData NewData;
    NewData.Dial = Dial;
    NewData.CurrentValue = GetDialValue(Dial);
    NewData.TargetValue = (NewData.CurrentValue + Direction) % 12;
    if (NewData.TargetValue < 0) NewData.TargetValue += 12;
    NewData.bIsRotating = true;
    DialsToAnimate.Add(NewData);
}

int32 ACombinationLock::GetDialValue(UStaticMeshComponent* Dial)
{
    float Pitch = Dial->GetRelativeRotation().Roll;
    float Normalized = FMath::Fmod(Pitch, 360.0f);
    if (Normalized < 0) Normalized += 360.0f;
    return FMath::RoundToInt(Normalized / 30.0f) % 12;
}

float ACombinationLock::SwitchSelectedDial(int32 value)
{
    SwitchValue = (SwitchValue + value + 5) % 5;

    switch (SwitchValue)
    {
    case 0:
        SelectedDial = Dial1;
        UE_LOG(LogTemp, Warning, TEXT("Dial 1 Selected"));
        return -8.0;

    case 1:
        SelectedDial = Dial2;
        UE_LOG(LogTemp, Warning, TEXT("Dial 2 Selected"));
        return -4.0;

    case 2:
        SelectedDial = Dial3;
        UE_LOG(LogTemp, Warning, TEXT("Dial 3 Selected"));
        return 0.0;

    case 3:
        SelectedDial = Dial4;
        UE_LOG(LogTemp, Warning, TEXT("Dial 4 Selected"));
        return 4.0;

    case 4:
        SelectedDial = Dial5;
        UE_LOG(LogTemp, Warning, TEXT("Dial 5 Selected"));
        return 8.0;
    }
    return 0.0;
}

void ACombinationLock::TriggerEvent()
{
    OnSomethingHappened.Broadcast();
}
