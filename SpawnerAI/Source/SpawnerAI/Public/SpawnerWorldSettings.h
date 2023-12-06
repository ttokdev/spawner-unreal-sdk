// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/WorldSettings.h"
#include "SpawnerWorldSettings.generated.h"

/**
 * 
 */
UCLASS()
class SPAWNERAI_API ASpawnerWorldSettings : public AWorldSettings
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category="Spawner Api Settings")
	bool bEnableSpawnerApi = true;
};
