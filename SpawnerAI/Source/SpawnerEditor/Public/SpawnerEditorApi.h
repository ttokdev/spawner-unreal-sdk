// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "SpawnerEditorApi.generated.h"

/**
 * 
 */
UCLASS()
class SPAWNEREDITOR_API USpawnerEditorApiSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
	/** Subsystem interface */
	virtual bool DoesSupportWorldType(EWorldType::Type WorldType) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
};
