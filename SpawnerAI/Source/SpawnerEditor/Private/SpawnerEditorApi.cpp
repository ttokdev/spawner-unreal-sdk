// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnerEditorApi.h"

bool USpawnerEditorApiSubsystem::DoesSupportWorldType(EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::Editor;
}

void USpawnerEditorApiSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, TEXT("USpawnerEditorApiSubsystem::Initialize!"));
}

void USpawnerEditorApiSubsystem::Deinitialize()
{
	Super::Deinitialize();
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, TEXT("USpawnerEditorApiSubsystem::Deinitialize!"));
}
