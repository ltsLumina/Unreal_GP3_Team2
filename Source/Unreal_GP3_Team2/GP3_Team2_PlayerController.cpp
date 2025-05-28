#include "GP3_Team2_PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/LevelStreaming.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerStart.h"

AGP3_Team2_PlayerController::AGP3_Team2_PlayerController()
{
    ActiveLevel = nullptr;
    PendingDeactivateLevel = nullptr;
    PendingActivateLevel = nullptr;
}

void AGP3_Team2_PlayerController::BeginPlay()
{
    Super::BeginPlay();

    if (GetPawn())
    {
        GetPawn()->Destroy();
    }

    // Check for level
    if (GetWorld()->GetName() == TEXT("StreamWorld"))
    {
        // Load Present level (Present)
        Present = LoadStreamingLevel(this, FName("L_Present_Vecna"), true);
        if (Present)
        {
            Present->OnLevelLoaded.AddDynamic(this, &AGP3_Team2_PlayerController::PresentLoaded);
            Present->OnLevelShown.AddDynamic(this, &AGP3_Team2_PlayerController::PresentLevelShown);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to load Present"));
        }

        // Load Past level (Past)
        Past = LoadStreamingLevel(this, FName("L_Past_Vecna"), false);
        if (Past)
        {
            Past->OnLevelLoaded.AddDynamic(this, &AGP3_Team2_PlayerController::PastLoaded);
            Past->OnLevelShown.AddDynamic(this, &AGP3_Team2_PlayerController::PastLevelShown);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to load Past"));
        }

        UE_LOG(LogTemp, Log, TEXT("Loading Levels - Present: %s, Past: %s"),
            Present ? *Present->GetFName().ToString() : TEXT("Failed"),
            Past ? *Past->GetFName().ToString() : TEXT("Failed"));
    }
    else
    {
        ACharacter* MyCharacter = Cast<ACharacter>(UGameplayStatics::GetActorOfClass(this, ACharacter::StaticClass()));

        if (MyCharacter)
        {
            if (APlayerStart* PlayerStart = Cast<APlayerStart>(UGameplayStatics::GetActorOfClass(this, APlayerStart::StaticClass())))
            {
                MyCharacter->SetActorLocation(PlayerStart->GetActorLocation());
            }

            Possess(MyCharacter);
        }
    }
}

ULevelStreaming* AGP3_Team2_PlayerController::LoadStreamingLevel(UObject* WorldContextObject, const FName& LevelName, bool bActive)
{
    if (!WorldContextObject || !WorldContextObject->GetWorld())
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid WorldContextObject or World for loading level: %s"), *LevelName.ToString());
        return nullptr;
    }

    UWorld* World = WorldContextObject->GetWorld();
    ULevelStreaming* StreamingLevel = UGameplayStatics::GetStreamingLevel(World, LevelName);
    if (!StreamingLevel)
    {
        UE_LOG(LogTemp, Error, TEXT("Streaming level not found: %s"), *LevelName.ToString());
        return nullptr;
    }

    if (!StreamingLevel->IsLevelLoaded())
    {
        StreamingLevel->SetShouldBeLoaded(true);
        StreamingLevel->bShouldBlockOnLoad = false;
        StreamingLevel->SetShouldBeVisible(bActive);
    }

    return StreamingLevel;
}

void AGP3_Team2_PlayerController::StartFade(bool bFadeIn, float Duration)
{
    if (!IsValid(this))
    {
        UE_LOG(LogTemp, Error, TEXT("StartFade: PlayerController is invalid"));
        return;
    }

    FColor FadeColor = FColor::Black;

    if (bFadeIn)
    {
        // Fade IN: from fully opaque black to transparent (visible)
        ClientSetCameraFade(true, FadeColor, FVector2D(1.f, 0.f), Duration, false, false);
    }
    else
    {
        // Fade OUT: from transparent to fully opaque black
        ClientSetCameraFade(true, FadeColor, FVector2D(0.f, 1.f), Duration, false, false);
    }
}

void AGP3_Team2_PlayerController::ChangeActiveWorld(ULevelStreaming* InActivate, ULevelStreaming* Activate)
{
    if (!InActivate || !Activate)
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid level for world change"));
        return;
    }

    PendingDeactivateLevel = InActivate;
    PendingActivateLevel = Activate;

    // Fade out first
    StartFade(false, 0.5f);

    // Set timer to delay actual swap until fade completes
    GetWorld()->GetTimerManager().SetTimer(FadeTimerHandle, this, &AGP3_Team2_PlayerController::OnFadeOutComplete, 0.6f, false);
}

void AGP3_Team2_PlayerController::OnFadeOutComplete()
{
    if (!PendingDeactivateLevel || !PendingActivateLevel)
    {
        UE_LOG(LogTemp, Warning, TEXT("Pending levels are invalid"));
        return;
    }

    // Deactivate old level
    if (ULevel* LoadedLevel = PendingDeactivateLevel->GetLoadedLevel())
    {
        PendingDeactivateLevel->SetShouldBeVisible(false);

        UE_LOG(LogTemp, Log, TEXT("Deactivating level: %s"), *PendingDeactivateLevel->GetFName().ToString());

        for (AActor* Actor : LoadedLevel->Actors)
        {
            if (IsValid(Actor))
            {
                Actor->SetActorEnableCollision(false);
                if (Actor == GetPawn())
                {
                    UnPossess();
                }
            }
        }
    }

    // Activate new level
    if (ULevel* LoadedLevel = PendingActivateLevel->GetLoadedLevel())
    {
        PendingActivateLevel->SetShouldBeVisible(true);
        UE_LOG(LogTemp, Log, TEXT("Activating level: %s"), *PendingActivateLevel->GetFName().ToString());

        StartFade(true, 1.0f); // Fade in
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Level not yet loaded: %s"), *PendingActivateLevel->GetFName().ToString());
        StartFade(true, 1.0f); // Ensure fade in anyway
    }

    // Clear pending levels and timer
    PendingDeactivateLevel = nullptr;
    PendingActivateLevel = nullptr;

    GetWorld()->GetTimerManager().ClearTimer(FadeTimerHandle);
}

void AGP3_Team2_PlayerController::SwapActiveLevel()
{
    if (!Present || !Past)
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot swap levels: One or both levels are invalid"));
        return;
    }

    if (ActiveLevel == Present)
    {
        ChangeActiveWorld(Present, Past);
        ActiveLevel = Past;
    }
    else
    {
        ChangeActiveWorld(Past, Present);
        ActiveLevel = Present;
    }
}

void AGP3_Team2_PlayerController::PastLoaded()
{
    if (Past && Past->GetLoadedLevel())
    {
        Past->SetShouldBeVisible(false);
        for (AActor* Actor : Past->GetLoadedLevel()->Actors)
        {
            if (IsValid(Actor))
            {
                Actor->SetActorEnableCollision(false);
            }
        }
        UE_LOG(LogTemp, Log, TEXT("Past level loaded: %s"), *Past->GetFName().ToString());
    }
}

void AGP3_Team2_PlayerController::PresentLoaded()
{
    if (Present && Present->GetLoadedLevel())
    {
        Present->SetShouldBeVisible(true);
        ActiveLevel = Present;

        ACharacter* TargetCharacter = nullptr;
        for (AActor* Actor : Present->GetLoadedLevel()->Actors)
        {
            if (IsValid(Actor))
            {
                Actor->SetActorEnableCollision(true);
                if (ACharacter* character = Cast<ACharacter>(Actor))
                {
                    if (!TargetCharacter)
                    {
                        TargetCharacter = character;
                    }
                    else
                    {
                        UE_LOG(LogTemp, Warning, TEXT("Multiple characters found in Present level"));
                    }
                }
            }
        }

        if (TargetCharacter && GetPawn() != TargetCharacter)
        {
            UE_LOG(LogTemp, Log, TEXT("Possessing character in Present: %s"), *TargetCharacter->GetName());
            Possess(TargetCharacter);
        }
    }
}

void AGP3_Team2_PlayerController::PresentLevelShown()
{
    if (Present && Present->IsLevelVisible())
    {
        ACharacter* TargetCharacter = nullptr;
        for (AActor* Actor : Present->GetLoadedLevel()->Actors)
        {
            if (IsValid(Actor))
            {
                Actor->SetActorEnableCollision(true);
                if (ACharacter* character = Cast<ACharacter>(Actor))
                {
                    if (!TargetCharacter)
                    {
                        TargetCharacter = character;
                    }
                    else
                    {
                        UE_LOG(LogTemp, Warning, TEXT("Multiple characters found in Present level"));
                    }
                }
            }
        }

        if (TargetCharacter && GetPawn() != TargetCharacter)
        {
            UE_LOG(LogTemp, Log, TEXT("Possessing character in Present: %s"), *TargetCharacter->GetName());
            Possess(TargetCharacter);
        }
        UE_LOG(LogTemp, Log, TEXT("Present level is now visible: %s"), *Present->GetFName().ToString());
    }
}

void AGP3_Team2_PlayerController::PastLevelShown()
{
    if (Past && Past->IsLevelVisible())
    {
        ACharacter* TargetCharacter = nullptr;
        for (AActor* Actor : Past->GetLoadedLevel()->Actors)
        {
            if (IsValid(Actor))
            {
                Actor->SetActorEnableCollision(true);
                if (ACharacter* character = Cast<ACharacter>(Actor))
                {
                    if (!TargetCharacter)
                    {
                        TargetCharacter = character;
                    }
                    else
                    {
                        UE_LOG(LogTemp, Warning, TEXT("Multiple characters found in Past level"));
                    }
                }
            }
        }

        if (TargetCharacter && GetPawn() != TargetCharacter)
        {
            UE_LOG(LogTemp, Log, TEXT("Possessing character in Past: %s"), *TargetCharacter->GetName());
            Possess(TargetCharacter);
        }
        UE_LOG(LogTemp, Log, TEXT("Past level is now visible: %s"), *Past->GetFName().ToString());
    }
}
