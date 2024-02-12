// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SpawnerCharacterComponent.generated.h"

class USpawnerApiSubsystem;

UCLASS( ClassGroup=(Spawner), meta=(BlueprintSpawnableComponent) )
class SPAWNERAI_API USpawnerCharacterComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCharacterTextStart, const FString&, Text, bool, bIsEnd);
	UPROPERTY(BlueprintAssignable, Category = "EventDispatcher")
	FOnCharacterTextStart OnCharacterTextStreamReceived;

	USpawnerCharacterComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintPure, Category = "Spawner")
	FORCEINLINE FString GetCharacterId() { return CharacterId; } const

	UFUNCTION(BlueprintCallable, Category = "Spawner")
	FORCEINLINE void SetCharacterId(const FString& id) { CharacterId = id; }

protected:
	virtual void BeginPlay() override;
private:
	UPROPERTY(EditAnywhere, Category = "Spawner")
	FString CharacterId;

	TWeakObjectPtr<USpawnerApiSubsystem> SpawnerApiSubsystem;
};
