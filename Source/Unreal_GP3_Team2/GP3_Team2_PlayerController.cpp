#include "GP3_Team2_PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/LevelStreaming.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerStart.h"

AGP3_Team2_PlayerController::AGP3_Team2_PlayerController()
{
    ActiveLevel = nullptr; // Initialize to avoid undefined behavior
}

void AGP3_Team2_PlayerController::BeginPlay()
{
    Super::BeginPlay();
    GetPawn()->Destroy();
    //Check for level
    if (GetWorld()->GetName() == TEXT("StreamWorld"))
    {
        // Load Present level (Present)
        Present = LoadStreamingLevel(this, FName("L_Present"), true);
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
        Past = LoadStreamingLevel(this, FName("L_Past"), false);
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

        if (APlayerStart * PlayerStart = Cast<APlayerStart>(UGameplayStatics::GetActorOfClass(this, APlayerStart::StaticClass())))
        {
            MyCharacter->SetActorLocation(PlayerStart->GetActorLocation());
        }

        Possess(MyCharacter);
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

void AGP3_Team2_PlayerController::ChangeActiveWorld(ULevelStreaming* InActivate, ULevelStreaming* Activate)
{
    if (!InActivate || !Activate)
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid level for world change"));
        return;
    }

    // Deactivate the current level
    if (ULevel* LoadedLevel = InActivate->GetLoadedLevel())
    {
        InActivate->SetShouldBeVisible(false);

        UE_LOG(LogTemp, Log, TEXT("Deactivating level: %s"), *InActivate->GetFName().ToString());

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

    //Activate the new level
    if (ULevel* LoadedLevel = Activate->GetLoadedLevel())
    {
        Activate->SetShouldBeVisible(true);
        UE_LOG(LogTemp, Log, TEXT("Activating level: %s"), *Activate->GetFName().ToString());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Level not yet loaded: %s"), *Activate->GetFName().ToString());
    }
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
        UE_LOG(LogTemp, Log, TEXT("Past level is now visible: %s"), *Past->GetFName().ToString());
    }
}