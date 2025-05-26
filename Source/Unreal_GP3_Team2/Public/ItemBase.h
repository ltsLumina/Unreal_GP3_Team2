// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemBase.generated.h"

UENUM(BlueprintType)
enum class EInteractionOptions : uint8
{
	NONE = 0, PICKUP = 1, INTERACT = 2, STASH = 3, READ = 4
};

USTRUCT(BlueprintType)
struct FInteractionOption
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString InteractionName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTexture2D> ControlImage;
};

UCLASS()
class UNREAL_GP3_TEAM2_API AItemBase : public APawn
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Details")
	FName ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Details")
	FText Description; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Details")
	UTexture2D* ItemIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Details")
	int32 ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Details")
	TArray<FInteractionOption> HighlightInteractionOptions;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void LookEnter();

	void LookExit();

private:
	bool _hasEntered = false;
	bool _hasLeft = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintNativeEvent)
	void OnLookEnter();

	void OnLookEnter_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	void OnLookExit();

	void OnLookExit_Implementation();

	UFUNCTION(BlueprintCallable)
	virtual TArray<FInteractionOption> GetInteractOptions();



	UFUNCTION()
	virtual void Look();

	UFUNCTION()
	virtual void LookAway();


};
