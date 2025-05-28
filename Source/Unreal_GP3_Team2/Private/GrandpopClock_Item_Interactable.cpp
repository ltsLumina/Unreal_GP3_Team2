// Fill out your copyright notice in the Description page of Project Settings.


#include "GrandpopClock_Item_Interactable.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

AGrandpopClock_Item_Interactable::AGrandpopClock_Item_Interactable()
{
    PrimaryActorTick.bCanEverTick = true;

    PresentWorld = false;

    FirstTime = 0;
    SecondTime = 0;

    // Create root component
    Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent = Root;

    // Create and attach components
    MainBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Clock"));
    MainBody->SetupAttachment(Root);

    BigPointer = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MinutePointer"));
    BigPointer->SetupAttachment(MainBody);

    SmallPointer = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HourPointer"));
    SmallPointer->SetupAttachment(MainBody);

    Pendulum = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pendulum"));
    Pendulum->SetupAttachment(MainBody);

}

void AGrandpopClock_Item_Interactable::BeginPlay()
{
    Super::BeginPlay();

    FString LevelName = GetWorld()->GetMapName();
    LevelName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

    PresentWorld = LevelName.Equals("L_Present_Vecna");

    if (PresentWorld)
    {
        Pendulum->SetVisibility(false);
        Pendulum->SetComponentTickEnabled(false);
        ItemName = "Brooken Clock";
        SetTimeInRotation(6, 0, FirstTime, SecondTime);
        BigPointer->SetRelativeRotation(FRotator(SecondTime, 0.f, 0.f));
        SmallPointer->SetRelativeRotation(FRotator(FirstTime, 0.f, 0.f));
    }
    else
    {
        ItemName = "Working Clock";
    }

    // Start timer
    GetWorld()->GetTimerManager().SetTimer(
        PendulumTimerHandle,
        this,
        &AGrandpopClock_Item_Interactable::SwitchPendulumDirection,
        SwingPeriod / 2.0f,
        true
    );
}

void AGrandpopClock_Item_Interactable::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!PresentWorld)
    {
        // Update pendulum
        UpdatePendulum(DeltaTime);

        // Update clock hands
        float HourRotation, MinuteRotation;
        TranslateTimeToRotation(HourRotation, MinuteRotation);

        // Apply rotations to clock
        SmallPointer->SetRelativeRotation(FRotator(HourRotation, 0.0f, 0.0f));
        BigPointer->SetRelativeRotation(FRotator(MinuteRotation, 0.0f, 0.0f));
    }
    if (PresentWorld && bShouldRotate)
    {
        SetTimeInRotation(8, 45, FirstTime, SecondTime);
        SetTargetRotation(DeltaTime, FirstTime, SecondTime);
    }
}

void AGrandpopClock_Item_Interactable::UpdatePendulum(float DeltaTime)
{
    ElapsedTime += DeltaTime;

    float TargetAngle = MaxSwingAngle * FMath::Sin((2 / SwingPeriod) * ElapsedTime);

    FRotator NewRotation(TargetAngle, 0.0f, 0.0f);
    Pendulum->SetRelativeRotation(NewRotation);
}


void AGrandpopClock_Item_Interactable::SwitchPendulumDirection()
{
    bIsSwingingForward = !bIsSwingingForward;
}

void AGrandpopClock_Item_Interactable::TranslateTimeToRotation(float& OutHourRotation, float& OutMinuteRotation)
{
    FDateTime Now = FDateTime::Now();
    int32 Hour = Now.GetHour();
    int32 Minutes = Now.GetMinute();
    int32 Seconds = Now.GetSecond();

    OutHourRotation = (Hour % 12 + Minutes / 60.0f) * 30.0f;
    OutMinuteRotation = (Minutes + Seconds / 60.0f) * 6.0f;
}

void AGrandpopClock_Item_Interactable::SetTargetRotation(float DeltaTime, float TargetHourRotation, float TargetMinuteRotation)
{
    if (!bShouldRotate) return;

    float CurrentHour = SmallPointer->GetRelativeRotation().Pitch;
    float CurrentMinute = BigPointer->GetRelativeRotation().Pitch;

    float HourDelta = FMath::FindDeltaAngleDegrees(CurrentHour, TargetHourRotation);
    float MinuteDelta = FMath::FindDeltaAngleDegrees(CurrentMinute, TargetMinuteRotation);

    float NewHour = CurrentHour + HourDelta * DeltaTime * RotationInterpSpeed;
    float NewMinute = CurrentMinute + MinuteDelta * DeltaTime * RotationInterpSpeed;

    SmallPointer->SetRelativeRotation(FRotator(NewHour, 0.f, 0.f));
    BigPointer->SetRelativeRotation(FRotator(NewMinute, 0.f, 0.f));

    float Threshold = 1.0f;
    bool bHourDone = FMath::Abs(HourDelta) < Threshold;
    bool bMinuteDone = FMath::Abs(MinuteDelta) < Threshold;

    //UE_LOG(LogTemp, Warning, TEXT("Hour diff: %f, Minute diff: %f"), HourDelta, MinuteDelta);

    if (bHourDone && bMinuteDone)
    {
        bShouldRotate = false;
    }
}

void AGrandpopClock_Item_Interactable::SetTimeInRotation(int32 Hour, int32 Minute, float& OutHourRotation, float& OutMinuteRotation)
{
    OutHourRotation = Hour * 30.0f;
    OutMinuteRotation = Minute * 6.0f;
    /*UE_LOG(LogTemp, Warning, TEXT("Hour %d with a Rotation of: %.1f, Minute %d with a Rotation of: %.1f"),
        Hour, OutHourRotation, Minute, OutMinuteRotation);*/
}

void AGrandpopClock_Item_Interactable::RepairClock()
{
    if (PresentWorld)
    {
        bShouldRotate = true;
        /*SetTimeInRotation(8, 45, FirstTime, SecondTime);
        SmallPointer->SetRelativeRotation(FRotator(FirstTime, 0.f, 0.f));
        BigPointer->SetRelativeRotation(FRotator(SecondTime, 0.f, 0.f));*/
        Pendulum->SetVisibility(true);
        Pendulum->SetComponentTickEnabled(true);
        ItemName = "Strange Clock";
        UE_LOG(LogTemp, Warning, TEXT("Interact Triggerd"));
    }
}