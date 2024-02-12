// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpawnerApiSubsystem.h"
#include "Components/ActorComponent.h"
#include "SpawnerPlayerComponent.generated.h"


UCLASS( ClassGroup=(Spawner), meta=(BlueprintSpawnableComponent) )
class SPAWNERAI_API USpawnerPlayerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USpawnerPlayerComponent();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Spawner")
	FORCEINLINE FString GetPlayerId() { return PlayerId; } const

	UFUNCTION(BlueprintCallable, Category = "Spawner")
	FORCEINLINE void SetPlayerId(const FString& Id) { PlayerId = Id; }

	UFUNCTION(BlueprintCallable, Category = "Spawner")
	FORCEINLINE FString GetPlayerName() { return PlayerName; } const
	
	UFUNCTION(BlueprintCallable, Category = "Spawner")
	FORCEINLINE void SetPlayerName(const FString& Name) { PlayerName = Name; }

	UFUNCTION(BlueprintCallable, Category = "Spawner")
	FORCEINLINE FString GetPlayerDescription() { return PlayerDescription; } const

	UFUNCTION(BlueprintCallable, Category = "Spawner")
	FORCEINLINE void SetPlayerDescription(const FString& Description) { PlayerDescription = Description; }

	UFUNCTION(BlueprintCallable, Category = "Spawner")
	FORCEINLINE FString GetPlayerAgeGroup() { return PlayerAgeGroup; } const

	UFUNCTION(BlueprintCallable, Category = "Spawner")
	FORCEINLINE void SetPlayerAgeGroup(const FString& AgeGroup) { PlayerAgeGroup = AgeGroup; }

	UFUNCTION(BlueprintCallable, Category = "Spawner")
	FORCEINLINE FString GetPlayerGender() { return PlayerGender; } const

	UFUNCTION(BlueprintCallable, Category = "Spawner")
	FORCEINLINE void SetPlayerGender(const FString& Gender) { PlayerGender = Gender; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	TWeakObjectPtr<USpawnerApiSubsystem> SpawnerApiSubsystem;

	UPROPERTY(EditAnywhere, Category = "Spawner")
	FString PlayerId;
	UPROPERTY(EditAnywhere, Category = "Spawner")
	FString PlayerName;
	UPROPERTY(EditAnywhere, Category = "Spawner")
	FString PlayerDescription;
	UPROPERTY(EditAnywhere, Category = "Spawner")
	FString PlayerAgeGroup;
	UPROPERTY(EditAnywhere, Category = "Spawner")
	FString PlayerGender;
};
